/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 13:46:49 by pecastro          #+#    #+#             */
/*   Updated: 2025/11/01 17:05:07 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "msh_builtins.h"
#include "msh_compile_cmds.h"

#include "libft.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

void	wait_child_processes(t_cd *pcd, int *pids, int ncmds)
{
	int	*wstatus;
	int	i;

	wstatus = malloc(sizeof(int) * ncmds);
	if (!wstatus)
	{
		//free pids, epaths etc?  cleanup cmds? and return(?);
	}
	i = 0;
	while (i < ncmds)
	{
		waitpid (pids[i], &wstatus[i], 0);
		if (WIFEXITED(wstatus[i]))
			pcd->exit_status = WEXITSTATUS(wstatus[i]);
		i ++;
	}
	free (wstatus);
	free(pids); //free pids here or in calling function?
}

static int	reset_stdiofds(t_cmd *curcmd)
{
	dup2(curcmd->stdio[0], STDIN_FILENO);
	dup2(curcmd->stdio[1], STDOUT_FILENO);
	close(curcmd->stdio[0]);
	close(curcmd->stdio[1]);
	return (1);
}

static int	execute_builtin(t_cd *pcd, t_cmd *cmd)
{
	cmd->stdio[0] = dup(STDIN_FILENO);
	cmd->stdio[1] = dup(STDOUT_FILENO);
	if (!setup_redirections(cmd->iolst))
		return (0);
	msh_builtins(pcd, cmd->args);
	reset_stdiofds(cmd);
	return (1);
}

static void	prepare_external_cmd(t_cd *pcd, t_cmd *cmd, char **epaths, size_t epaths_len)
{
	char	*cmdexe;

	if (!ft_is_builtin(cmd->args[0]))
	{
		cmdexe = compose_check_cmd(pcd, cmd, epaths, epaths_len);
		free (cmd->args[0]);
		cmd->args[0] = cmdexe;
	}
}

static int	process_single_cmd(t_cd *pcd, t_cmd *cmd, t_cmd_ctx *ctx, int *idx)
{
	if (!cmd->args || !cmd->args[0])
		return (1);
	if (ctx->childless)
		return (execute_builtin(pcd, cmd));
	else
	{
		prepare_external_cmd(pcd, cmd, ctx->epaths, ctx->epaths_len);
		parent_setup(pcd, cmd, ctx->pids, (*idx)++);
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

int	init_cmd_ctx(t_cmd_ctx *ctx, t_cd *pcd, char const *in)
{
	ctx->cmds = msh_compile_cmds(pcd, in, pcd->default_sqed);
	if (!ctx->cmds || !ctx->cmds->args || !ctx->cmds->args[0] ||
		 ctx->cmds->args[0][0] == '#')
		 return (0);
	ctx->epaths = ft_env_paths(&ctx->epaths_len);
	ctx->ncmds = count_cmds(ctx->cmds);
	ctx->childless = (ctx->ncmds == 1 && ft_is_builtin(ctx->cmds->args[0]));
	if (!ctx->childless)
	{
		ctx->pids = malloc(sizeof(int) * ctx->ncmds);
		if (!ctx->pids)
			return (0);
	}
	return (1);
}