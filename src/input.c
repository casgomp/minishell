/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 11:35:05 by rhaas             #+#    #+#             */
/*   Updated: 2025/10/17 12:31:54 by rhaas            ###   ########.fr       */
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
	// char **const	cmdln_tokens = rh_split_rmqed(in, pcd->default_sqed);
	// List of commands
	t_cmd *const	cmds = msh_compile_cmds(pcd, in, pcd->default_sqed);
	t_cmd 			*curcmd;
	int				p_fd[2][2];
	char			**epaths;
	size_t			epaths_len;
	char			*cmdexe;


	epaths = ft_env_paths(&epaths_len);
	if (cmds && cmds->args && cmds->args[0] && cmds->args[0][0] != '#')
	{
		curcmd = cmds;
		while (curcmd)
		{
			//maybe a small function for setting cmdexe as args[0]?
			cmdexe = compose_check_cmd(pcd, curcmd, epaths, epaths_len);
			free(curcmd->args[0]);
			curcmd->args[0] = cmdexe;

			
			char **const	args = curcmd->args;
			//function run cmd(subset_args)
			if (args && args[0])
			{
				if (!msh_builtins(pcd, args))
				{
					// run extern executable using fork and exec...
					//check if there's pipes (other ctrl chrs behave differently i.e. for N commands && will create N-1 children, not N)
					if (curcmd->ctrlchr == '|')
					{
						//pipe
						if (pipe(p_fd[?][?]) == -1)
							p_fd[?][?] = -1;
						close(p_fd[?][?]); //how to know 1st cmd or middle or end cmd to know what to close? 

						
						//fork ->go to child?
						//close ...only with pipes parent calls close.
						//wait
					}
					//fork ->go to child
					//wait
					//next step happens inside child
					while (curcmd && curcmd->iolst)
					{
						//curcmd->iolst = curcmd->iolst->next??
					}
					// Dummy
					msh_error((char const *[]){"Command not found Dummy Error..."}, 1, -1);
					return (0);
				}
			}
			//free stuff in curcmd?
			curcmd = curcmd->next;
		}
	}
	free(epaths);
	return (/*cleanup cmds with ft_splitstrs, pcd->exit_status = 0 ,*/ 1);
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
		t_sqed const sqed = pcd->default_sqed;
		t_qed const	qed = (t_qed){.q = sqed.q, .esc = sqed.esc, .expnd = sqed.expnd,
					.dict = sqed.dict, .dict_data = sqed.dict_data,
					.validchar = sqed.validchar,
					.validchar_data = sqed.validchar_data};

		pprompt = rh_rmqed("$USER@minishell$\\ ", qed/* pcd->default_sqed */);
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
