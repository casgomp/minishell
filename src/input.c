/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 11:35:05 by rhaas             #+#    #+#             */
/*   Updated: 2025/10/15 20:14:50 by rhaas            ###   ########.fr       */
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
	t_cmd *const	cmds = msh_compile_cmds(in, pcd->default_sqed);
	t_cmd 			*curcmd;
	int				(*pipe_fd)[2];
	char			**epaths;
	char			*path;
	int				epaths_len;
	int				bufferlen;

	epaths = ft_env_paths(&epaths_len);
	if (cmds && cmds->args && cmds->args[0] && cmds->args[0][0] != '#')
	{
		curcmd = cmds;
		while (curcmd)
		{
			bufferlen = 0;
			bufferlen = epaths_len + ft_strlen(curcmd->args[0]) + 1;
			path = ft_calloc(bufferlen, sizeof(char));
			if (!path)
				//return NULL? and set cd->exit to what?
			if (curcmd->ctrlchr == '|')//check if there's pipes (other ctrl chrs behave differently i.e. for N commands && will create N-1 children, not N)
			{
				//pipe
				
				
				//fork ->go to child?
				//close ...only with pipes parent calls close.
				//wait
			}
			else if (curcmd->iolst->tredirect != rd_default)
			{
				//fork ->go to child
				//wait
			}
			else
			{
				//call compose_check_cmd which should call stat and access from within
				//check stat.
				path = cmd_verif_s(curcmd, epaths); 
				if (!path)
				{
					//
				}
				//check access.
				if (!accessvariable??)
				{
					free(path); //?? free or now should be NULL?
				}
			}

			
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
