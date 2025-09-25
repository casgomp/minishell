/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 11:35:05 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/25 20:04:06 by rhaas            ###   ########.fr       */
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

int process_cmdline(t_cd *pcd, char const *const in)
{
	char **const cmdln_tokens = rh_split_rmqed(in, pcd->default_sqed);

	if (cmdln_tokens && *cmdln_tokens)
	{
		// loop over pipe sections
		{
			// do io redirect
			// stdin from file, heredoc or pipe
			// stdout to file trunc|append or pipe
			// compose cmdln_tokens array for current command

			char **args = cmdln_tokens;
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
	return (rh_free_simple_array((void **)cmdln_tokens)/*necessary?*/, 0);
}

int	setup_stdin(t_cd *pcd, int const argc, char **const argv)
{
	pcd->fdscript = -1;
	if (argc == 2)
	{
		pcd->fdscript = open(argv[1], O_RDONLY);
		if (pcd->fdscript == -1)
		{
			perror(argv[1]);
			exit (127);
		}
		else if (dup2(pcd->fdscript, 0) == -1)
		{
			perror(argv[1]);
			exit (9);
		}
	}
	else if (argc > 2)
	{
		ft_printf("usage: minishell [scriptfile]\n");
		exit (7);
	}
	pcd->interactive = isatty(0);
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
