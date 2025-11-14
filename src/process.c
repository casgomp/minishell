/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 13:46:49 by pecastro          #+#    #+#             */
/*   Updated: 2025/11/14 10:45:44 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# define _GNU_SOURCE
# define _POSIX_C_SOURCE 200809L
# include "minishell.h"
# include "msh_builtins.h"
# include "msh_compile_cmds.h"

# include "libft.h"

# include <signal.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <sys/wait.h>
# include <sys/types.h>

void	wait_child_processes(t_cd *pcd, int *pids, int ncmds)
{
	int	*wstatus;
	int	i;
	struct sigaction	sa_ignore;
	struct sigaction	old_sigint;

	wstatus = malloc(sizeof(int) * ncmds);
	if (!wstatus)
	{
		free (pids);
		return ;
	}
	ft_memset(&sa_ignore, 0, sizeof(sa_ignore));
	sa_ignore.sa_handler = SIG_IGN;
	sigemptyset(&sa_ignore.sa_mask);
	sa_ignore.sa_flags = 0;
	sigaction(SIGINT, &sa_ignore, &old_sigint);
	i = 0;
	while (i < ncmds)
	{
		waitpid (pids[i], &wstatus[i], 0);
		if (WIFEXITED(wstatus[i]))
			pcd->exit_status = WEXITSTATUS(wstatus[i]);
		else if (WIFSIGNALED(wstatus[i]))
		{
			pcd->exit_status = 128 + WTERMSIG(wstatus[i]);
			if (WTERMSIG(wstatus[i]) == SIGINT)
				write(1, "\n", 1);
			else if (WTERMSIG(wstatus[i]) == SIGQUIT && WCOREDUMP(wstatus[i]) && i == ncmds - 1)
					write(1, "Quit (core dumped)\n", 19);
		}
		i ++;
	}
	sigaction(SIGINT, &old_sigint, NULL);
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

void	prepare_external_cmd(t_cd *pcd, t_cmd *cmd, char **epaths, size_t epaths_len)
{
	char	*cmdexe;

	if (!ft_is_builtin(cmd->args[0]))
	{
		cmdexe = compose_check_cmd(pcd, cmd, epaths, epaths_len);
		//if !cmdexe return 0?
		free (cmd->args[0]);
		cmd->args[0] = cmdexe;
	}
}

static int	process_single_cmd(t_cd *pcd, t_cmd *cmd, t_cmd_ctx *ctx, int *idx)
{
	//if (!cmd->args || !cmd->args[0])//maybe also if idx != ncmds - 1, because if last command empty it waits for a cmd in stdin. then i can return 0.
	if (!cmd->args)
		return (1);// cant return 0 because if last command empty it waits for a cmd in stdin...bash returns syntax error message and exit status 2.
	if (ctx->childless)
		return (execute_builtin(pcd, cmd));
	else
	{
		//prepare_external_cmd(pcd, cmd, ctx->epaths, ctx->epaths_len);
		//if (!parent_setup(pcd, cmd, ctx->pids, (*idx)++))
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

int	init_cmd_ctx(t_cmd_ctx *ctx, t_cd *pcd, char const *in)
{
	ctx->cmds = msh_compile_cmds(pcd, in, pcd->default_sqed);

	/////////////////////////////////////////////////////
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
	/////////////////////////////////////////////////////

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