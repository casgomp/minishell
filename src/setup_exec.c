/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 13:50:29 by pecastro          #+#    #+#             */
/*   Updated: 2025/11/23 16:26:34 by rhaas            ###   ########.fr       */
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
		exit (127);
	ft_memset(&sa, 0, sizeof(sa));
	if (ft_is_builtin(curcmd->args[0]))
	{
		msh_builtins(pcd, curcmd->args);
		exit(0);
	}
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
	dup2(curredir->old_fd, curredir->new_fd);
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
	//ft_printf("fail_open_closefds?\n");
	msh_error((char const *[]){curredir->fname_delim, "No such file or directory"}, 2, 0);
	return (1);
}

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
			return (msh_error((char const *[]){iolst->fname_delim}, 1, errno), 0);
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

//void	prepare_external_cmd(t_cd *pcd, t_cmd *cmd, char **epaths, size_t epaths_len)
int	prepare_external_cmd(t_cd *pcd, t_cmd *cmd, t_cmd_ctx *ctx, char **path_noaccess)
{
	if (!ft_is_builtin(cmd->args[0]))
	{
		if (!compose_check_cmd(pcd, cmd, ctx, path_noaccess))
		{
			//printf("prepare_external_cmd: errno = %d\n", errno);
			return (0);
		}
	}
	return (1);
}

int	child_setup(t_cd *pcd, t_cmd *curcmd, t_fdcl *fdcl, int *fdpipe, t_cmd_ctx *ctx)
{
	char	*path_noaccess;

	path_noaccess = NULL;
	g_signal_received = 0;
	inchild_pipe_setup(fdcl, fdpipe);
	if (!setup_redirections(curcmd->iolst))
		exit (1);
	if (!prepare_external_cmd(pcd, curcmd, ctx, &path_noaccess))
	{
		if (pcd->exit_status == 126)
			msh_error((char const *[]){path_noaccess, "Permission denied"}, 2, 0);
		if (pcd->exit_status == 127)
		{	
			if (ft_strchr(curcmd->args[0], '/') || ctx->epaths[0] == NULL)
				msh_error((char const *[]){curcmd->args[0], "No such file or directory"}, 2, 0);
			else
				msh_error((char const *[]){curcmd->args[0], "command not found"}, 2, 0);
		}
		if (path_noaccess)
			free(path_noaccess);
		exit (pcd->exit_status);
	}
	else
		child_cmd_execute(pcd, curcmd);
	if (path_noaccess)
			free(path_noaccess);
	return (1);
}

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

//int	parent_setup(t_cd *pcd, t_cmd *curcmd, int *pids, int i)
int	parent_setup(t_cd *pcd, t_cmd *curcmd, t_cmd_ctx *ctx, int i)
{
	static t_fdcl	fdcl = {{-1, -1}, {-1, -1}};
	int				fdpipe[2] = {-1, -1};
	//t_io			*curredir;

	if (fdcl.pre2[0] != -1)
		close(fdcl.pre2[0]);
	if (fdcl.pre1[1] != -1)
		close(fdcl.pre1[1]);
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
		child_setup(pcd, curcmd, &fdcl, fdpipe, ctx);
		exit(1);
	}
	/*if (fdcl.pre1[0] != -1)
		close(fdcl.pre1[0]);
	if (fdpipe[1] != -1)
		close(fdpipe[1]);
	curredir = curcmd->iolst;
	while (curredir)
	{
		if (curredir->old_fd != -1)
		{
			close (curredir->old_fd);
			curredir->old_fd = -1;
		}
		curredir = curredir->next;
	}
	fdcl.pre2[0] = fdcl.pre1[0];
	fdcl.pre2[1] = fdcl.pre1[1];
	fdcl.pre1[0] = fdpipe[0];
	fdcl.pre1[1] = fdpipe[1];*/
	parent_reset_after_fork(&fdcl, fdpipe, curcmd->iolst);
	return (1);
}
