/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 16:20:50 by pecastro          #+#    #+#             */
/*   Updated: 2025/11/03 17:33:05 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# define _POSIX_C_SOURCE 200809L
# include "minishell.h"
# include "libft.h"

# include <errno.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
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
			return (1);
		}
		if (ft_strncmp(ret, limiter, len) == 0 && ret[len] == '\n')
			return (free(ret), 1);
		if (write(fd, ret, ft_strlen(ret)) == -1)
			return (free(ret), 0);
		free (ret);
	}
}

int	heredoc_setup(t_io *curredir)
{
	int					hdfdpipe[2];
	int					ret;
	struct sigaction	sa;

	if (!setup_signal_heredoc(&sa, sigint_handler_heredoc))
		exit(1); //perror?
	while (curredir)
	{	
		if (curredir->tredirect == heredoc2fd)
		{
			if (pipe(hdfdpipe) == -1)
			{
				//perror("pipe");//msh_error?
				exit (1);
			}
			g_signal_received = 0;
			ret = heredoc_prompt(hdfdpipe[1], curredir->fname_delim);
			if (!ret)
			{
				close (hdfdpipe[1]);
				exit (128 + SIGINT);
			}
			curredir->old_fd = hdfdpipe[0];
		}
		curredir = curredir->next;
	}
	reset_handlers_heredoc(&sa);
	return (1);
}