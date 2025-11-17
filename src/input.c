/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 11:35:05 by rhaas             #+#    #+#             */
/*   Updated: 2025/11/17 21:08:22 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "msh_builtins.h"
#include "msh_compile_cmds.h"

#include "libft.h"

#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int	count_cmds(t_cmd *curcmd)
{
	int	ncmds;

	ncmds = 0;
	while (curcmd)
	{
		ncmds ++;
		curcmd = curcmd->next;
	}
	return (ncmds);
}

int	ft_is_builtin(char *cmd)
{
	int			i;
	const char	*bltn[] = {
		"cd",
		"echo",
		"env",
		"exit",
		"export",
		"pwd",
		"unset",
		NULL
	};

	if (cmd)
	{
		i = 0;
		while (bltn[i])
		{
			if (ft_strcmp(cmd, bltn[i]) == 0)
				return (true);
			i ++;
		}
	}
	return (false);
}

int	process_cmdline(t_cd *pcd)
{
	t_cmd_ctx	ctx;
	int			ret;

	ft_memset(&ctx, 0, sizeof(t_cmd_ctx));
	if (!init_cmd_ctx(&ctx, pcd))
		return (0);
	ret = process_all_cmds(pcd, &ctx);
	if (!ctx.childless && ret)
		wait_child_processes(pcd, ctx.pids, ctx.ncmds);
	else if (!ctx.childless && !ret)
	{
		if (ctx.pids)
			free(ctx.pids);
	}
	if (ctx.epaths) //free pids too?
		free_splitstrs(ctx.epaths);
	cleanup_cmds(&ctx.cmds);
	return (ret);
}

int	setup_stdin(t_cd *pcd, int const argc, char **const argv)
{
	if (argc == 2)
	{
		pcd->interactive = false;
		if (pcd->fdscript >= 0)
			close(pcd->fdscript);
		pcd->fdscript = open(argv[1], O_RDONLY);
		if (pcd->fdscript == -1)
		{
			perror(argv[1]);
			msh_builtins(pcd, (char *[]){"exit", "127"});
		}
		else if (dup2(pcd->fdscript, 0) == -1)
		{
			perror(argv[1]);
			msh_builtins(pcd, (char *[]){"exit", "9"});
		}
	}
	else if (argc > 2)
	{
		ft_printf("usage: minishell [scriptfile]\n");
		msh_builtins(pcd, (char *[]){"exit", "7"});
	}
	return (1);
}

static void	get_cmdline_addhist_prompt(t_cd *pcd, char **pcmdline)
{
	t_qed				qed;
	char				*prompt;
	char const *const	prompts[] = {"$USER@minishell$ ", "> "};

	qed = rh_sqed2qed(&(pcd->default_sqed));
	prompt = rh_rmqed(prompts[(int)pcd->pt], &qed);
	if (prompt)
	{
		*pcmdline = readline(prompt);
		free(prompt);
		if (*pcmdline && (*pcmdline)[0])
			add_history(*pcmdline);
	}
	else
	{
		pcd->exit_status = 130;
		msh_error((char const *[]){"Prompt generation error"}, 1, -1);
	}
}

int	get_cmdline_add_history(t_cd *pcd, char **pcmdline)
{
	char	*cmdlnsection;

	if (!pcmdline)
		return (0);
	*pcmdline = (char *) NULL;
	if (pcd->interactive)
		get_cmdline_addhist_prompt(pcd, pcmdline);
	else
	{
		while (true)
		{
			cmdlnsection = get_next_line(0);
			if (!cmdlnsection)
				break ;
			if (!(*pcmdline))
				*pcmdline = "";
			*pcmdline = ft_strjoin(*pcmdline, cmdlnsection);
			free (cmdlnsection);
		}
	}
	if (!*pcmdline)
		return (pcd->exit = true, 0);
	return (1);
}
