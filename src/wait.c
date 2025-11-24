/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 13:46:49 by pecastro          #+#    #+#             */
/*   Updated: 2025/11/21 14:13:10 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _POSIX_C_SOURCE 200809L
#include "minishell.h"
#include "msh_builtins.h"
#include "msh_compile_cmds.h"
#include "libft.h"
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

static void	exit_status_wait(t_cd *pcd, t_cmd_ctx *ctx, 
	t_cmd *curcmd, int wstatus_i)
{
	pcd->exit_status = WEXITSTATUS(wstatus_i);
	ctx->parpth = 1;
	compose_check_cmd(pcd, curcmd, ctx);
	if (curcmd->path)
		free (curcmd->path);
	if (pcd->exit_status == 126)
		msh_error((char const *[]){ctx->path_noaccess, 
			"Permission denied"}, 2, 0);
	if (pcd->exit_status == 127)
	{
		if (ft_strchr(curcmd->args[0], '/') || ctx->epaths[0] == NULL)
			msh_error((char const *[]){curcmd->args[0], 
				"No such file or directory"}, 2, 0);
		else
			msh_error((char const *[]){curcmd->args[0], 
				"command not found"}, 2, 0);
	}
	if (ctx->path_noaccess)
		free(ctx->path_noaccess);
}

int	wait_child_processes(t_cd *pcd, t_cmd_ctx *ctx)
{
	int					*wstatus;
	int					i;
	struct sigaction	old_sigint;
	int					sgntcnt;
	t_cmd				*curcmd;

	wstatus = malloc(sizeof(int) * ctx->ncmds);
	if (!wstatus)
		return (free (ctx->pids), 0);
	setup_signal_wait(&old_sigint);
	sgntcnt = 0;
	i = 0;
	curcmd = ctx->cmds;
	while (curcmd)
	{
		waitpid (ctx->pids[i], &wstatus[i], 0);
		if (WIFEXITED(wstatus[i]))
			exit_status_wait(pcd, ctx, curcmd, wstatus[i]);
		else if (WIFSIGNALED(wstatus[i]))
			sigint_handler_wait(pcd, wstatus[i], ctx->ncmds, i, &sgntcnt);
		i ++;
		curcmd = curcmd->next;
	}
	return (sigaction(SIGINT, &old_sigint, NULL), 
		free (wstatus), free(ctx->pids), 1);
}