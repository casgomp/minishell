/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 11:35:05 by rhaas             #+#    #+#             */
/*   Updated: 2025/10/25 19:25:22 by rhaas            ###   ########.fr       */
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
#include <sys/wait.h>

static void	print_info_cmds(t_cmd *const cmds) //just for debugging
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
}

int	count_children(t_cmd *curcmd)
{
	int	nchild;

	nchild = 0;
	ft_printf("count_children: nchild= %d\n", nchild);//debugging
	if (curcmd->ctrlchr == '|')
			nchild ++;
	while (curcmd)
	{
		if (curcmd->ctrlchr == '|' || curcmd->iolst)
			nchild ++;
		curcmd = curcmd->next;
	}
	ft_printf("count_children: nchild= %d\n", nchild);//debugging
	return (nchild);
}

int	process_cmdline(t_cd *pcd, char const *const in)
{
	t_cmd *const	cmds = msh_compile_cmds(pcd, in, pcd->default_sqed);
	t_cmd 			*curcmd;
	char			**epaths;
	size_t			epaths_len;
	char			*cmdexe;
	int				nchild;
	int				*pids;
	int				i;
	int				*wstatus;

	print_info_cmds(cmds);
	ft_printf("process_cmdline: %s\n", cmds->args[0]);//debugging
	epaths = ft_env_paths(&epaths_len);
	nchild = 0;
	pids = 0;
	i = -1;
	if (cmds && cmds->args && cmds->args[0] && cmds->args[0][0] != '#')
	{
		curcmd = cmds;
		while (curcmd)
		{
			ft_printf("process_cmdline: curcmd->args[0]= %s\n", curcmd->args[0]);//debugging
			//would getting cmdexe happen in children. And maybe put in small function?
			cmdexe = compose_check_cmd(pcd, curcmd, epaths, epaths_len);
			free(curcmd->args[0]);
			curcmd->args[0] = cmdexe;
			ft_printf("process_cmdline: curcmd->args[0]= %s, cmdexe= %s, pcd->exit_status= %d\n", curcmd->args[0], cmdexe, pcd->exit_status);//debugging
			
			//if heredocs in parent, all must be stored in a char *buffer[N]

			char **const	args = curcmd->args;
			//function run cmd(subset_args).
			ft_printf("process_cmdline: args[0]= %s\n", args[0]);//debugging
			if (args && args[0]) //wrong? redirections happen regardless of stat/access return.
			{
				if (!msh_builtins(pcd, args))//wrong? this way I cant get cdmxe after in children.
				{
					nchild = count_children(curcmd);
					ft_printf("process_cmdline: nchild= %d\n", nchild);//debugging
					if (nchild == 0)
					{
						//cmdexe in child? then also here for simple cmd.
						execve(args[0], args, environ);
						perror(curcmd->args[0]);//what? wrong filename or options?
						return (0);//return what and set exits status?
					}
					pids = malloc(sizeof(int) * nchild);
					if (!pids)
						return (1);//return (NULL)? and free something?
					parent_setup(curcmd, pids, ++i);
					//child_setup gets called from parent but 
					//could be better to call here?
					//also, should get cmdexe in child?

					// Dummy
					//msh_error((char const *[]){"Command not found Dummy Error..."}, 1, -1);
					//return (0);
				}
			}
			//free stuff in curcmd?
			curcmd = curcmd->next;
		}
		//close opened fds?
		wstatus = malloc(sizeof(int) * nchild);
		if (!wstatus)
		{
			//free? and return(?);
		}
		i = 0;
		while (i < nchild)
		{
			waitpid (pids[i], &wstatus[i], 0);
			if (WIFEXITED(wstatus[i]))
				pcd->exit_status = WEXITSTATUS(wstatus[i]);
			i ++;
		}
		free (wstatus);
		free(pids);
		return (1);
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
