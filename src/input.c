/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 11:35:05 by rhaas             #+#    #+#             */
/*   Updated: 2025/10/30 10:19:23 by rhaas            ###   ########.fr       */
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

/*static void	print_info_cmds(t_cmd *const cmds) //just for debugging
{
	t_cmd	*currcmd = cmds;

	while (currcmd)
	{
		ft_printf("print_info_cmds: args[0]=%s, ctrlchr=%c\n", currcmd->args[0], currcmd->ctrlchr);
		t_io	*curredir = currcmd->iolst;
		int i = 0;
		while (curredir)
		{
			ft_printf("print_info_redir: loop %d; sign_enum=%d, fnm_delim=%s\n", i, curredir->tredirect, curredir->fname_delim);	
			curredir = curredir->next;
			i ++;
		}
		currcmd = currcmd->next;
	}
}*/
/*
int	count_children(t_cmd *curcmd)
{
	int	ncmds;

	ncmds = 0;
	ft_printf("count_children: ncmds= %d\n", ncmds);//debugging
	if (curcmd->ctrlchr == '|')
			ncmds ++;
	while (curcmd)
	{
		if (curcmd->ctrlchr == '|' || curcmd->iolst)
			ncmds ++;
		curcmd = curcmd->next;
	}
	ft_printf("count_children: ncmds= %d\n", ncmds);//debugging
	return (ncmds);
}*/

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
	char 	**bltn;
	int		i;
	int		res;

	bltn = malloc(sizeof(char *) * 8);
	if (!bltn)
		return (false);
	bltn[0] = "cd";
	bltn[1] = "echo";
	bltn[2] = "env";
	bltn[3] = "exit";
	bltn[4] = "export";
	bltn[5] = "pwd";
	bltn[6] = "unset";
	bltn[7] = NULL;
	res = false;
	i = 0;
	while (bltn[i])
	{
		if (ft_strcmp(cmd, bltn[i]) == 0)
		{
			res = true;
			return (free(bltn), res);
		}
		i ++;
	}
	return (free(bltn), res);
}

int	process_cmdline(t_cd *pcd, char const *const in)
{
	t_cmd *const	cmds = msh_compile_cmds(pcd, in, pcd->default_sqed);
	t_cmd 			*curcmd;
	char			**epaths;
	size_t			epaths_len;
	char			*cmdexe;
	int				ncmds;
	int				*pids;
	int				i;
	int				*wstatus;
	int				childless;
	t_io			*curredir;
	int				ret;

	epaths = ft_env_paths(&epaths_len);
	ncmds = 0;
	pids = 0;
	i = 0;
	childless = false;
	ret = 1;
	if (cmds && cmds->args && cmds->args[0] && cmds->args[0][0] != '#')
	{
		curcmd = cmds;
		ncmds = count_cmds(curcmd);
		if (ncmds == 1 && ft_is_builtin(curcmd->args[0]))
			childless = true;
		if (!childless)
		{
			pids = malloc(sizeof(int) * ncmds);
			if (!pids) //malloc must return a non-null pointer otherwise if ncmds=0, pids=NULL and !pids=true.
				return (1);//return (NULL)? and free something?
		}
		while (curcmd)
		{
			//would getting cmdexe happen in children. And maybe put in small function?
			/*cmdexe = compose_check_cmd(pcd, curcmd, epaths, epaths_len);
			free(curcmd->args[0]);
			curcmd->args[0] = cmdexe;*/

			//if heredocs in parent, all must be stored in a char *buffer[N]

			char **const	args = curcmd->args;
			//function run cmd(subset_args).
			if (args && args[0])
			{
				if (childless)
				{
					//do redirections backup
					curcmd->stdio[0] = dup(STDIN_FILENO);
					curcmd->stdio[1] = dup(STDOUT_FILENO);
					curredir = curcmd->iolst;
					while (curredir && (ret))
					{
						ret = redirections_setup(curredir);
						curredir = curredir->next;
					}
					if (ret)
						msh_builtins(pcd, curcmd->args);
					//restore redirections
					dup2(curcmd->stdio[0], STDIN_FILENO);
					dup2(curcmd->stdio[1], STDOUT_FILENO);
					close(curcmd->stdio[0]);
					close(curcmd->stdio[1]);
				}
				else
				{
					if (!ft_is_builtin(args[0]))
					{
						cmdexe = compose_check_cmd(pcd, curcmd, epaths, epaths_len);
						free(curcmd->args[0]);//prbbly bst nt rplce args[0] and instd save path (i.e. cmdexe) smwhre else
						curcmd->args[0] = cmdexe;
					}
					//if (!msh_builtins(pcd, args))
					
					//put compose check here.............
					/*if (ncmds == 0)
					{
						//cmdexe in child? then also here for simple cmd.
						execve(args[0], args, environ);
						perror(curcmd->args[0]);//what? wrong filename or options?
						return (0);//return what and set exits status?
					}*/
					parent_setup(pcd, curcmd, pids, i++);
					//child_setup gets called from parent but
					//could be better to call here?
					//also, should get cmdexe in child?

					// Dummy
					//msh_error((char const *[]){"Command not found Dummy Error..."}, 1, -1);
					//return (0);
				}
			}
			//free stuff in curcmd?
			//free pids?
			curcmd = curcmd->next;
		}
		if (!childless)
		{
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
			free(pids); //if pids free pids.
		}
	}
	//free epaths?
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
