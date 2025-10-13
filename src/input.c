/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 11:35:05 by rhaas             #+#    #+#             */
/*   Updated: 2025/10/13 18:47:05 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
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
	// char **const	cmdln_tokens = rh_split_rmqed(in, pcd->default_sqed);
	// List of commands
	t_cmd *const	cmds = msh_compile_cmds(in, pcd->default_sqed);
	t_cmd 			*curcmd;
	
	//store paths from environ.
	some_struct.epaths = ft_env_paths(); //or maybe not a struct.
	//Before entering loop:
		//check if there are redirections
		//check if there are pipes
	//If no pipes and no redirections, then simple cmd 

	if (cmds && cmds->args && cmds->args[0] && cmds->args[0][0] != '#') //# can happen anywhere in a command (not just [0]). Bash takes care of it at parsing, because it can impact number of pipes to open.
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
					char **tmp = rh_split_rmqed("$SHELL", pcd->default_sqed);
					ft_printf("%s: %s: Command not found Dummy Error...\n ...until a better solution ;)\n",
						tmp[0], args[0]);
					return (
						rh_free_simple_array((void **)tmp),
						/*cleanup cmds; */
						0);
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
			msh_builtins(pcd, (char* []){"exit", "127"});
		}
		else if (dup2(pcd->fdscript, 0) == -1)
		{
			perror(argv[1]);
			msh_builtins(pcd, (char* []){"exit", "9"});
		}
	}
	else if (argc > 2)
	{
		ft_printf("usage: minishell [scriptfile]\n");
		msh_builtins(pcd, (char* []){"exit", "7"});
	}
	return (1);
}

int	get_cmdline_add_history(t_cd *pcd, char **pcmdline)
{
	char		**pprompt;
	size_t		cmdln_len;

	if (!pcmdline)
		return (0);
	if (pcd->interactive)
	{
		pprompt = rh_split_rmqed("$USER@minishell$\\ ", pcd->default_sqed);
		if (pprompt && *pprompt)
		{
			*pcmdline = readline(*pprompt);
			rh_free_simple_array((void **)pprompt);
			if (*pcmdline)
				add_history(*pcmdline);
		}
	}
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
