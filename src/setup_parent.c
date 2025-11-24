/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_parent.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 13:50:29 by pecastro          #+#    #+#             */
/*   Updated: 2025/11/20 18:55:39 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// # define _POSIX_C_SOURCE 200809L
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

void	parent_reset_after_fork(t_fdcl *fdcl, int *fdpipe, t_io *curredir)
{
	if (fdcl->pre1[0] != -1)
		close(fdcl->pre1[0]);
	if (fdpipe[1] != -1)
		close(fdpipe[1]);
	while (curredir)
	{
		if (curredir->old_fd != -1)
		{
			close (curredir->old_fd);
			curredir->old_fd = -1;
		}
		curredir = curredir->next;
	}
	fdcl->pre2[0] = fdcl->pre1[0];
	fdcl->pre2[1] = fdcl->pre1[1];
	fdcl->pre1[0] = fdpipe[0];
	fdcl->pre1[1] = fdpipe[1];
}

static void	parent_setup_init(t_fdcl *fdcl, int *fdpipe)
{
	fdpipe[0] = -1;
	fdpipe[1] = -1;
	if (fdcl->pre2[0] != -1)
		close(fdcl->pre2[0]);
	if (fdcl->pre1[1] != -1)
		close(fdcl->pre1[1]);
}

int	parent_setup(t_cd *pcd, t_cmd *curcmd, t_cmd_ctx *ctx, int i)
{
	static t_fdcl	fdcl = {{-1, -1}, {-1, -1}};
	int				fdpipe[2];

	parent_setup_init(&fdcl, fdpipe);
	if (!heredoc_setup(curcmd->iolst, pcd))
		return (0);
	if (curcmd->ctrlchr == '|')
	{
		if (pipe(fdpipe) == -1)
			return (perror("pipe"), 0);
	}
	ctx->pids[i] = fork();
	if (ctx->pids[i] == -1)
		return (perror("fork"), 0);
	if (ctx->pids[i] == 0)
	{
		g_signal_received = 0;
		inchild_pipe_setup(&fdcl, fdpipe);
		child_setup(pcd, curcmd, ctx);
		exit(1);
	}
	return (parent_reset_after_fork(&fdcl, fdpipe, curcmd->iolst), 1);
}
