/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 13:46:49 by pecastro          #+#    #+#             */
/*   Updated: 2025/11/20 17:35:30 by rhaas            ###   ########.fr       */
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

static int	reset_stdiofds(t_cmd *curcmd)
{
	dup2(curcmd->stdio[0], STDIN_FILENO);
	dup2(curcmd->stdio[1], STDOUT_FILENO);
	dup2(curcmd->stdio[2], STDERR_FILENO);
	close(curcmd->stdio[0]);
	close(curcmd->stdio[1]);
	close(curcmd->stdio[2]);
	return (1);
}

static int	execute_builtin(t_cd *pcd, t_cmd *cmd)
{
	cmd->stdio[0] = dup(STDIN_FILENO);
	cmd->stdio[1] = dup(STDOUT_FILENO);
	cmd->stdio[2] = dup(STDERR_FILENO);
	if (!setup_redirections(cmd->iolst))
		return (0);
	msh_builtins(pcd, cmd->args);
	reset_stdiofds(cmd);
	return (1);
}

static int	process_single_cmd(t_cd *pcd, t_cmd *cmd, t_cmd_ctx *ctx, int *idx)
{
	if (!cmd->args)
		return (1);
	if (ctx->childless)
		return (execute_builtin(pcd, cmd));
	else
	{
		if (!parent_setup(pcd, cmd, ctx, (*idx)++))
			return (0);
		return (1);
	}
}

int	process_all_cmds(t_cd *pcd, t_cmd_ctx *ctx)
{
	t_cmd	*curcmd;
	int		i;
	int		ret;

	curcmd = ctx->cmds;
	i = 0;
	while (curcmd)
	{
		ret = process_single_cmd(pcd, curcmd, ctx, &i);
		if (!ret)
			return (0);
		curcmd = curcmd->next;
	}
	return (1);
}

int	init_cmd_ctx(t_cmd_ctx *ctx, t_cd *pcd)
{
	ctx->cmds = msh_compile_cmds(pcd, pcd->default_sqed, (char **) NULL);
	if (!ctx->cmds || !ctx->cmds->args)
		return (0);
	ctx->epaths = ft_env_paths(pcd, &ctx->epaths_len);
	ctx->ncmds = count_cmds(ctx->cmds);
	ctx->path_noaccess = NULL;
	ctx->parpth = 0;
	ctx->childless = (ctx->ncmds == 1 && ft_is_builtin(ctx->cmds->args[0]));
	if (!ctx->childless)
	{
		ctx->pids = malloc(sizeof(int) * ctx->ncmds);
		if (!ctx->pids)
			return (0);
	}
	return (1);
}
