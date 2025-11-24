/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 16:20:50 by pecastro          #+#    #+#             */
/*   Updated: 2025/11/24 15:33:36 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "msh_builtins.h"
#include "msh_compile_cmds.h"
#include "libft.h"

#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

static bool	heredoc_check_retval(/*t_cd *pcd,*/ char const *const ret,
	char const *const limiter)
{
	char	buf[512];

	(void)pcd;
	if (!ret)
	{
		if (g_signal_received == SIGINT)
			return (false);
		ft_sprintf(buf, "here-document delimited by "
			"end-of-file (wanted `%s')", limiter);
		msh_error((char const *[]){"warning", buf, }, 2, -1);
		return (false);
	}
	return (true);
}

int	heredoc_prompt(t_cd *pcd, int fd, char *limiter)
{
	char	*ret;
	int		len;

	ret = NULL;
	len = ft_strlen(limiter);
	while (true)
	{
		msh_compile_cmds_get_cmdlines(pcd, &ret, (t_cs)cs_uninitialised);
		if (g_signal_received == SIGINT)
			return (0);
		if ((!heredoc_check_retval(pcd, ret, limiter))
			|| (ret && ft_strncmp(ret, limiter, len) == 0 && ret[len] == '\0'))
			return (true);
		if (ft_dprintf(fd, "%s\n", ret) < 0)
			return (false);
	}
}

static int	terminal_safe_heredoc_prompt(t_cd *pcd, int fd, char *limiter)
{
	int	ret;
	int	stdin_safe;

	stdin_safe = dup(STDIN_FILENO);
	pcd->pt = (t_pt)pt_contd;
	pcd->in_heredoc = true;//
	ret = heredoc_prompt(pcd, fd, limiter);
	pcd->in_heredoc = false;//
	pcd->pt = (t_pt)pt_std;
	close(fd);
	close(STDIN_FILENO);
	dup2(stdin_safe, STDIN_FILENO);
	return (ret);
}

void	heredoc_setup_clean_pipe_fail(t_cd *pcd, t_sa *sa_old)
{
	msh_error((char const *[]){"pipe"}, 1, errno);
	if (sa_old->sig_modif)
		reset_handlers_heredoc(&sa_old->sa_int, &sa_old->sa_quit);
	pcd->exit_status = 1;
}

int	heredoc_setup_send_to_prompt(t_cd *pcd, t_io *curredir,
	t_sa *sa_old, int *hdfdpipe)
{
	int	ret;

	ret = terminal_safe_heredoc_prompt(pcd, hdfdpipe[1], curredir->fname_delim);
	if (!ret)
	{
		close (hdfdpipe[0]);
		if (sa_old->sig_modif)
		{
			reset_handlers_heredoc(&sa_old->sa_int, &sa_old->sa_quit);
			sa_old->sig_modif = false;
		}
		if (g_signal_received == SIGINT)
		{
			pcd->exit_status = 130;
			//g_signal_received = 0;
		}
		else
			pcd->exit_status = 1;
		return (0);
	}
	curredir->old_fd = hdfdpipe[0];
	return (1);
}

int	heredoc_setup(t_io *curredir, t_cd *pcd)
{
	int					hdfdpipe[2];
	t_sa				sa_old;

	sa_old.sig_modif = false;
	while (curredir)
	{
		if (curredir->tredirect == heredoc2fd)
		{
			if (!sa_old.sig_modif)
			{
				if (!setup_signal_heredoc(&sa_old, sigint_handler_heredoc))
					return (pcd->exit_status = 1, 0);
				sa_old.sig_modif = true;
			}
			if (pipe(hdfdpipe) == -1)
				return (heredoc_setup_clean_pipe_fail(pcd, &sa_old), 0);
			g_signal_received = 0;
			if (!heredoc_setup_send_to_prompt(pcd, curredir, &sa_old, hdfdpipe))
				return (0);
		}
		curredir = curredir->next;
	}
	if (sa_old.sig_modif)
		reset_handlers_heredoc(&sa_old.sa_int, &sa_old.sa_quit);
	return (1);
}
