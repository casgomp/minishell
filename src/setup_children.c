/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_children.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 13:50:29 by pecastro          #+#    #+#             */
/*   Updated: 2025/11/20 19:12:14 by rhaas            ###   ########.fr       */
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

int	open_files_setup(t_io *curredir)
{
	char	*fnmdlm;

	fnmdlm = curredir->fname_delim;
	if (curredir->tredirect == fd2file_trunc)
		curredir->old_fd = open(fnmdlm, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	else if (curredir->tredirect == fd2file_append)
		curredir->old_fd = open(fnmdlm, O_WRONLY | O_CREAT | O_APPEND, 0666);
	else if (curredir->tredirect == file2fd)
		curredir->old_fd = open(fnmdlm, O_RDONLY);
	else if (curredir->tredirect == heredoc2fd)
		return (1);
	if (curredir->old_fd == -1)
		return (0);
	return (1);
}

int	setup_redirections(t_io *iolst)
{
	t_io	*curredir;
	int		ret;

	curredir = iolst;
	ret = 1;
	while (curredir && ret != 0)
	{
		ret = open_files_setup(curredir);
		if (ret == 0)
			return (fail_open_closefds(iolst), 0);
		if (apply_redirection(curredir) == 0)
			return (msh_error((char const *[]){iolst->fname_delim}, 
				1, errno), 0);
		curredir = curredir->next;
	}
	return (1);
}

int	inchild_pipe_setup(t_fdcl *fdcl, int *fdpipe)
{
	if (fdpipe[0] != -1)
		close(fdpipe[0]);
	if (fdcl->pre1[0] != -1)
	{
		dup2(fdcl->pre1[0], STDIN_FILENO);
		close(fdcl->pre1[0]);
	}
	if (fdpipe[1] != -1)
	{
		dup2(fdpipe[1], STDOUT_FILENO);
		close(fdpipe[1]);
	}
	return (1);
}

int	prepare_external_cmd(t_cd *pcd, t_cmd *cmd, t_cmd_ctx *ctx)
{
	if (!ft_is_builtin(cmd->args[0]))
	{
		if (!compose_check_cmd(pcd, cmd, ctx))
			return (0);
	}
	return (1);
}

int	child_setup(t_cd *pcd, t_cmd *curcmd, t_cmd_ctx *ctx)
{
	if (!setup_redirections(curcmd->iolst))
		exit (1);
	if (!prepare_external_cmd(pcd, curcmd, ctx))
		exit (pcd->exit_status);
	else
		child_cmd_execute(pcd, curcmd);
	return (1);
}
