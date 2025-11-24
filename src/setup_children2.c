/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_children2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 13:50:29 by pecastro          #+#    #+#             */
/*   Updated: 2025/11/24 22:34:27 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "msh_builtins.h"
#include "libft.h"

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int	child_cmd_execute(t_cd *pcd, t_cmd *curcmd)
{
	struct sigaction	sa;

	if (!curcmd->args || !curcmd->args[0])
		return (127);
	ft_memset(&sa, 0, sizeof(sa));
	if (ft_is_builtin(curcmd->args[0]))
		return (!msh_builtins(pcd, curcmd->args));
	else
	{
		if (curcmd->args[0])
		{
			sa.sa_handler = SIG_DFL;
			sigemptyset(&sa.sa_mask);
			sigaction(SIGINT, &sa, NULL);
			sigaction(SIGQUIT, &sa, NULL);
			execve(curcmd->path, curcmd->args, pcd->msh_env);
			perror(curcmd->args[0]);
		}
	}
	return (1);
}

int	apply_redirection(t_io *curredir)
{
	if (dup2(curredir->old_fd, curredir->new_fd) == -1)
	{
		msh_error((char const *[]){ft_itoa(curredir->new_fd),}, 1, errno);
		return (0);
	}
	close(curredir->old_fd);
	return (1);
}

int	fail_open_closefds(t_io *curredir)
{
	while (curredir && curredir->old_fd != -1)
	{
		close (curredir->old_fd);
		curredir = curredir->next;
	}
	msh_error((char const *[]){curredir->fname_delim, 
		"No such file or directory"}, 2, 0);
	return (1);
}
