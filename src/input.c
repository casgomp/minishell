/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 11:35:05 by rhaas             #+#    #+#             */
/*   Updated: 2025/10/23 17:24:43 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "msh_builtins.h"
#include "msh_rmquotes_expand.h"

#include "libft.h"

#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <fcntl.h>

int	process_cmdline(t_cd *pcd, char const *const in)
{
	t_cmd *const	cmds = msh_compile_cmds(pcd, in, pcd->default_sqed);
	t_cmd			*curcmd;

	if (cmds && cmds->args && cmds->args[0] && cmds->args[0][0] != '#')
	{
		curcmd = cmds;
		// loop over pipe sections
		while (curcmd)
		{
			// do io redirect
			// stdin from file, heredoc or pipe
			// stdout to file trunc|append or pipe
			// compose cmdln_tokens array for current command

			char **const	args = curcmd->args;
			//function run cmd(subset_args)
			if (args && args[0])
			{
				if (!msh_builtins(pcd, args))
				{
					// run extern executable using fork and exec...

					// Dummy
					msh_error((char const *[]){"Command not found Dummy Error..."}, 1, -1);
					return (0);
				}
			}
			curcmd = curcmd->next;
		}
	}
	return (/*cleanup cmds , pcd->exit_status = 0 ,*/ 1);
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
	t_qed	qed;
	char	*prompt;

	qed = rh_sqed2qed(&(pcd->default_sqed));
	prompt = rh_rmqed("$USER@minishell$ ", &qed);
	if (prompt)
	{
		*pcmdline = readline(prompt);
		free(prompt);
		if (*pcmdline)
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
	size_t		cmdln_len;

	if (!pcmdline)
		return (0);
	*pcmdline = (char *) NULL;
	if (pcd->interactive)
		get_cmdline_addhist_prompt(pcd, pcmdline);
	else
	{
		*pcmdline = get_next_line(0);
		if (*pcmdline)
		{
			cmdln_len = ft_strlen(*pcmdline);
			if (cmdln_len > 0 && (*pcmdline)[cmdln_len - 1] == '\n')
				(*pcmdline)[cmdln_len - 1] = '\0';
		}
	}
	if (!*pcmdline)
	{
		pcd->exit = true;
		return (0);
	}
	return (1);
}
