/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 16:20:50 by pecastro          #+#    #+#             */
/*   Updated: 2025/11/15 16:19:57 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# define _POSIX_C_SOURCE 200809L
# define _DEFAULT_SOURCE
# define _BSD_SOURCE
# include "minishell.h"
# include "libft.h"

# include <errno.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <termios.h>
# include <unistd.h>

int	heredoc_prompt(int fd, char *limiter)
{
	char	*ret;
	char	*prompt;
	int		len;

	len = ft_strlen(limiter);
	prompt = "> ";
	while (1)
	{
		if (g_signal_received == SIGINT)
			return (0);
		if (write(1, prompt, 2) == -1)
			return (0);
		ret = get_next_line(0);
		if (!ret)
		{
			if (g_signal_received == SIGINT)
				return (0);
			return (write (1, "\n", 1), 1);
		}
		if (ft_strncmp(ret, limiter, len) == 0 && ret[len] == '\n')
			return (free(ret), 1);
		if (write(fd, ret, ft_strlen(ret)) == -1)
			return (free(ret), 0);
		free (ret);
	}
}

static int	terminal_safe_heredoc_prompt(int fd, char *limiter)
{
	struct termios	saved_termios;
	struct termios	new_termios;
	int				ret;
	int				terminal_modified;

	terminal_modified = 0;
	if (isatty(STDIN_FILENO))
	{
		if (tcgetattr(STDIN_FILENO, &saved_termios) == -1)
			return (perror("tcgetattr"), 0);
		new_termios = saved_termios;
		new_termios.c_lflag &= ~ECHOCTL;
		if (tcsetattr(STDIN_FILENO, TCSANOW, &new_termios) == -1)
			return (perror("tcsetattr"), 0);
		terminal_modified = 1;
	}
	ret = heredoc_prompt(fd, limiter);
	if (isatty(STDIN_FILENO) && terminal_modified)
		tcsetattr(STDIN_FILENO, TCSANOW, &saved_termios);
	return (ret);
}

int	heredoc_setup(t_io *curredir, t_cd *pcd)
{
	int					hdfdpipe[2];
	int					ret;
	//struct sigaction	sa;
	struct sigaction	sa_old_int;
	struct sigaction	sa_old_quit;
	bool				sig_modif;

	sig_modif = false;
	while (curredir)
	{	
		if (curredir->tredirect == heredoc2fd)
		{
			if (!sig_modif)
			{
				if (!setup_signal_heredoc(&sa_old_int, &sa_old_quit, sigint_handler_heredoc))
					return (pcd->exit_status = 1, 0);
				sig_modif = true;
			}
			if (pipe(hdfdpipe) == -1)
			{
				//perror("pipe");//msh_error?
				//exit (1);
				if (sig_modif)
					reset_handlers_heredoc(&sa_old_int, &sa_old_quit);
				pcd->exit_status = 1;
				return (0);
			}
			g_signal_received = 0;
			ret = terminal_safe_heredoc_prompt(hdfdpipe[1], curredir->fname_delim);
			close(hdfdpipe[1]);//
			if (!ret)
			{
				close (hdfdpipe[0]);
				if (sig_modif)
				{
					reset_handlers_heredoc(&sa_old_int, &sa_old_quit);
					sig_modif = false;
				}
				pcd->exit_status = 128 + SIGINT;
				return (0);
			}
			curredir->old_fd = hdfdpipe[0];
		}
		curredir = curredir->next;
	}
	if (sig_modif)
		reset_handlers_heredoc(&sa_old_int, &sa_old_quit);
	return (1);
}