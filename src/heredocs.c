/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 16:20:50 by pecastro          #+#    #+#             */
/*   Updated: 2025/11/23 16:55:12 by rhaas            ###   ########.fr       */
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

static bool	heredoc_check_retval(t_cd *pcd, char const *const ret,
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

int	heredoc_prompt(t_cd *pcd, char *limiter)
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
		if (!heredoc_check_retval(pcd, ret, limiter))
			return (0);
		if (ret && ft_strncmp(ret, limiter, len) == 0 && ret[len] == '\0')
			return (1);
		if (ft_dprintf(1, "%s\n", ret) < 0)
			return (0);
	}
}

static int	terminal_safe_heredoc_prompt(t_cd *pcd, char *limiter)
{
	int	ret;
	int	stdin_safe;

	stdin_safe = dup(STDIN_FILENO);
	pcd->pt = (t_pt)pt_contd;
	ret = heredoc_prompt(pcd, limiter);
	pcd->pt = (t_pt)pt_std;
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

int	heredoc_setup_send_to_prompt(t_cd *pcd, t_io *curredir, t_sa *sa_old)
{
	int	ret;

	ret = terminal_safe_heredoc_prompt(pcd, curredir->fname_delim);
	if (!ret)
	{
		if (sa_old->sig_modif)
		{
			reset_handlers_heredoc(&sa_old->sa_int, &sa_old->sa_quit);
			sa_old->sig_modif = false;
		}
		pcd->exit_status = 128 + SIGINT;
		return (0);
	}
	return (1);
}

int	heredoc_setup(t_io *curredir, t_cd *pcd)
{
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
			g_signal_received = 0;
			if (!heredoc_setup_send_to_prompt(pcd, curredir, &sa_old))
				return (0);
		}
		curredir = curredir->next;
	}
	if (sa_old.sig_modif)
		reset_handlers_heredoc(&sa_old.sa_int, &sa_old.sa_quit);
	return (1);
}
