/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 13:46:49 by pecastro          #+#    #+#             */
/*   Updated: 2025/11/19 10:22:25 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# define _POSIX_C_SOURCE 200809L
# include "minishell.h"
# include "msh_builtins.h"
# include "msh_compile_cmds.h"

# include "libft.h"

# include <signal.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <unistd.h>

int	wait_child_processes(t_cd *pcd, int *pids, int ncmds)
{
	int					*wstatus;
	int					i;
	//struct sigaction	sa_ignore;
	struct sigaction	old_sigint;
	int					sgntcnt;	

	wstatus = malloc(sizeof(int) * ncmds);
	if (!wstatus)	
		return (free (pids), 0);
	setup_signal_wait(&old_sigint);
	sgntcnt = 0;
	i = 0;
	while (i < ncmds)
	{
		waitpid (pids[i], &wstatus[i], 0);
		if (WIFEXITED(wstatus[i]))
			pcd->exit_status = WEXITSTATUS(wstatus[i]);
		else if (WIFSIGNALED(wstatus[i]))
			sigint_handler_wait(pcd, wstatus[i], ncmds, i, &sgntcnt);
		i ++;
	}
	return (sigaction(SIGINT, &old_sigint, NULL), free (wstatus), free(pids), 1);
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

int	init_cmd_ctx(t_cmd_ctx *ctx, t_cd *pcd/* , char const *in */)
{
	ctx->cmds = msh_compile_cmds(pcd, pcd->default_sqed, (char **) NULL);

	/*////////////////////////////////////////////////////
	//debugging loop to see what msh_compile_cmds returns
	int	i = 0;
	t_cmd *curcmd = ctx->cmds;
	while (curcmd)
	{
		int	j = 0;
		char **curargs = curcmd->args;
		//ft_printf("cmd n.%d, arg n.%d = %s\n", i, j, curargs[j]);
		while (curargs[j])
		{
			ft_printf("cmd n.%d, arg n.%d = %s\n", i, j, curargs[j]);
			j ++;
		}
		t_io *curredir = curcmd->iolst;
		while (curredir)
		{
			ft_printf("fname_delim=%s, old_fd=%d, new_fd=%d, tredirect=%d\n", curredir->fname_delim, curredir->old_fd, curredir->new_fd, curredir->tredirect);
			curredir = curredir->next;
		}
		curcmd = curcmd->next;
		i ++;
	}
	/*/////////////////////////////////////////////////////

	//if (!ctx->cmds || !ctx->cmds->args || !ctx->cmds->args[0])
	if (!ctx->cmds || !ctx->cmds->args)
		 return (0);
	ctx->epaths = ft_env_paths(pcd, &ctx->epaths_len);
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