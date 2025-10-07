/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 11:35:05 by rhaas             #+#    #+#             */
/*   Updated: 2025/10/07 16:15:08 by pecastro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <fcntl.h>

int	process_cmdline(t_cd *pcd, char const *const in)
{
	char **const	cmdln_tokens = rh_split_rmqed(in, pcd->default_sqed);
	//new vars for redirections/pipes

	/*
	int i = 0;
	while (cmdln_tokens[i])
	{
		printf("process_cmdline: cmdln_tokens[%i] = %s\n", i, cmdln_tokens[i]);
		i ++;
	}*/
	
	if (cmdln_tokens && *cmdln_tokens)
	{
		// First thing for safety, dup stdin and stdout and store them in struct.
		// Then, loop over pipe sections:
		// Find every segment of the pipeline (i.e. separated by control operator)
		//what if instead of creating char *segment[], Create char *
		//i = 0;
		//while (cmdln_tokens[i])
		//	p_seg[i++] = ft_strdup(cmdln_tokens[i])

		// and put each segment into a separate array of strings.
		// Open files 
		// Then 

		{
			// do io redirect
			// stdin from file, heredoc or pipe
			// stdout to file trunc|append or pipe
			// compose cmdln_tokens array for current command

			char **const	args = cmdln_tokens;
			//function run cmd(subset_args)
			if (args && args[0])
			{
				if (ms_builtins(pcd, args))
					return (rh_free_simple_array((void **)cmdln_tokens), 1);
				else
				{
					// run extern executable using fork and exec...

					// Dummy
					char **tmp = rh_split_rmqed("$SHELL", pcd->default_sqed);
					ft_printf("%s: %s: Command not found Dummy Error...\n ...until a better solution ;)\n",
						tmp[0], args[0]);
					return (
						rh_free_simple_array((void **)tmp),
						rh_free_simple_array((void **)cmdln_tokens),
						0);
				}
			}
			else
			{
				// handle empty command string??
			}
		}
	}
	return (rh_free_simple_array((void **) cmdln_tokens), 0);
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
			ms_builtins(pcd, (char* []){"exit", "127"});
		}
		else if (dup2(pcd->fdscript, 0) == -1)
		{
			perror(argv[1]);
			ms_builtins(pcd, (char* []){"exit", "9"});
		}
	}
	else if (argc > 2)
	{
		ft_printf("usage: minishell [scriptfile]\n");
		ms_builtins(pcd, (char* []){"exit", "7"});
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
		pprompt = rh_split_rmqed("$USER@minishell$ ", pcd->default_sqed);
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
