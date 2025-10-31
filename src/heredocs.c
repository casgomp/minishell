/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 16:20:50 by pecastro          #+#    #+#             */
/*   Updated: 2025/10/31 18:42:41 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int	heredoc_prompt(int fd, char *limiter)
{
	char	*ret;
	char	*prompt;
	int		len;

	len = ft_strlen(limiter);
	prompt = "> ";
	while (1)
	{
		if (write(1, prompt, 2) == -1)
			return (0);
		ret = get_next_line(0);
		if (!ret)
			return (0);
		if (ft_strncmp(ret, limiter, len) == 0 && ret[len] == '\n')
			return (free(ret), 1);
		if (write(fd, ret, ft_strlen(ret)) == -1)
			return (free(ret), 0);
		free (ret);
	}
}

int	heredoc_setup(t_io *curredir)
{
	int		hdfdpipe[2];

	while (curredir)
	{	
		if (curredir->tredirect == heredoc2fd)
		{
			if (pipe(hdfdpipe) == -1)
			{
				perror("pipe");//msh_error?
				exit (1);
			}
			if (heredoc_prompt(hdfdpipe[1], curredir->fname_delim) == 0)
			{
				perror("heredoc");//msh_error?
				exit (1);
			}
			close (hdfdpipe[1]);
			curredir->old_fd = hdfdpipe[0];
		}
		curredir = curredir->next;
	}
	return (1);
}