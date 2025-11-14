/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_compile_cmds.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:12:30 by rhaas             #+#    #+#             */
/*   Updated: 2025/11/14 10:33:16 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "msh_builtins.h"
#include "msh_compile_cmds.h"
#include "msh_compile_cmds_intern.h"
#include "msh_defs.h"
#include "rh_split_qed.h"

#include <limits.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <unistd.h>

char	**msh_expand_wordsplit_token(char const *const token,
	t_sqed const *const psqed)
{
	char		*argword;
	char		**args;
	int			argidx;
	t_qed		qed;
	t_qes		qes4wordsplit;

	qed = rh_sqed2qed(psqed);
	qed.expstat = false;
	qed.rmqe = false;
	argword = rh_rmqed(token, &qed);
	qes4wordsplit = rh_sqed2qes_ws(psqed);
	if (!qed.expstat)
		qes4wordsplit.blank = "";
	args = rh_split_quotes(argword, qes4wordsplit);
	free(argword);
	qed = rh_sqed2qed_rmonly(psqed);
	argidx = 0;
	while (args && args[argidx])
	{
		argword = rh_rmqed(args[argidx], &qed);
		free(args[argidx]);
		args[argidx++] = argword;
	}
	return (args);
}

static int	msh_process_metachar(t_cd *pcd, t_tca *ptca,
	int *ptokenidx, t_sqed const sqed)
{
	int const	tokenidx = *ptokenidx;
	char const	*token = ptca->tokens[tokenidx];
	int const	tokenlen = ft_strlen(token);

	if (!ft_strchr(sqed.metachr, token[tokenlen - 1]))
		return (0);
	if (!ft_strcmp(token, "|"))
		ptca->curcmd->ctrlchr = '|';
	else if (!ft_strcmp(token, "\n"))
		ptca->curcmd->ctrlchr = '\n';
	else if (!ft_strcmp(token, ";"))
		ptca->curcmd->ctrlchr = ';';
	else if (ft_strchr("<>", token[tokenlen - 1]))
	{
		if (msh_process_ioredirector(pcd, ptca, tokenidx, sqed))
			++(*ptokenidx);
		else
			return (-1);
	}
	else
	{
		return (msh_syntaxerror(pcd, token, ptca->tosplit), -1);
	}
	return (1);
}

static bool	msh_process_simplecmd_token(t_cd *pcd, t_tca *ptca,
	int *ptokenidx, t_sqed const sqed)
{
	int		res;
	char	**args;

	res = msh_process_metachar(pcd, ptca, ptokenidx, sqed);
	if (res == -1)
		return (false);
	else if (res == 0)
	{
		args = msh_expand_wordsplit_token(ptca->tokens[*ptokenidx], &sqed);
		res = 0;
		while (args && args[res])
		{
			if (!get_curarg(ptca))
				return (false);
			ptca->curarg->arg = args[res++];
		}
	}
	return (true);
}

static t_cmd	*msh_compile_cmds_core(t_cd *pcd, char const *const cmdline,
			t_cmd *cmdlst_sofar, t_sqed const sqed)
{
	t_tca		tca;
	int			tokenidx;

	tca.tosplit = cmdline;
	tca.tokens = rh_split_quotes(cmdline, rh_sqed2qes(&sqed));
	tca.cmdlst = cmdlst_sofar;
	tca.arglst = (t_cmdarg *) NULL;
	tokenidx = 0;
	while (tca.tokens && (*tca.tokens) && tca.tokens[tokenidx])
	{
		if (tca.tokens[tokenidx][0] == '#')
		{
			int tkidx2 = tokenidx;
			while (tca.tokens[tkidx2])
			{
				free(tca.tokens[tkidx2]);				
				tca.tokens[tkidx2++] = (char *) NULL;
			}
			if (tokenidx == 0)
				break ;
		}
		else
		{
			if (!set_curcmd(&tca))
				return (cleanup_tca(&tca), NULL);
			if (!msh_process_simplecmd_token(pcd, &tca, &tokenidx, sqed))
				return (cleanup_tca(&tca), (t_cmd *) NULL);
			++tokenidx;				
		}
		if (tca.curcmd->ctrlchr || !tca.tokens[tokenidx])
		{
			if (!convert_arglst(&tca))
				return (cleanup_tca(&tca), (t_cmd *) NULL);
			cleanup_arglst(&tca);
		}
	}
	return (cleanup_tca(&tca), tca.cmdlst);
}

static int	msh_check_cmds(t_cd *const pcd, t_cmd const *const cmds,
	char const *const cmdline, t_cmd const **cleanincl)
{
	t_cmd const	*cmd = cmds;
	
	while (cmd)
	{
		if (cmd->ctrlchr == '|')
		{
			if (!(cmd->args && cmd->args[0]))
			{
				*cleanincl = cmd;
				return (msh_syntaxerror(pcd, "|", cmdline), MSH_SYNTAX_ERR);
			}
			// // Possibly handle this case differently and
			// // continue reading from prompt until msh_check_cmds 
			// // is happy?
			if (!cmd->next)
			{
				// return (msh_syntaxerror(pcd, "newline", cmdline), false);
				return (MSH_UNFINISHED_PIPE);
			}
		}
		cmd = cmd->next;
	}
	return (MSH_CMDS_OK);
}

static void cleanup_iolst(t_io **piolst_head)
{
	t_io	*elem;

	if (!piolst_head)
		return ;
	while (*piolst_head)
	{
		elem = (*piolst_head)->next;
		free(*piolst_head);
		(*piolst_head) = elem;
	}
	(*piolst_head) = (t_io *) NULL;
}

static void	cleanup_cmds(t_cmd **pcmds_head, t_cmd const *clean_incl)
{
	t_cmd	*cmd;
	int		stop_cleanup;

	stop_cleanup = 0;
	if (!pcmds_head)
		return ;
	while (*pcmds_head && stop_cleanup < 2)
	{
		rh_free_simple_array((void ***)(&((*pcmds_head)->args)));
		cleanup_iolst(&((*pcmds_head)->iolst));
		stop_cleanup += ((*pcmds_head) == clean_incl);
		stop_cleanup += (stop_cleanup && ((*pcmds_head)->ctrlchr == '\n'));
		cmd = (*pcmds_head)->next;
		free(*pcmds_head);
		(*pcmds_head) = cmd;
	}
}

t_cmd	*msh_compile_cmds(t_cd *pcd, t_sqed const sqed)
{
	t_cmd	*cmds = (t_cmd *) NULL;
	t_cmd const	*cleanincl = (t_cmd *) NULL;
	char	*cmdln = (char *) NULL;
	static char	**cmdlnsec = (char **) NULL;
	static int	cmdlnsecidx = 0;
	int		ret;
	t_pt	pt;

	pt = (t_pt)pt_std;
	while (!pcd->exit)
	{
		if (!(cmdlnsec && cmdlnsec[cmdlnsecidx]))
		{
			if (!get_cmdline_add_history(pcd, &cmdln, pt))
				return (cleanup_cmds(&cmds, cleanincl), cmds);
			cmdlnsec = rh_split_quotes(cmdln, rh_sqed2qes_cmdln(&pcd->default_sqed));
			if (!(cmdlnsec && cmdlnsec[0]))
			{
				rh_free_simple_array((void ***)&cmdlnsec);
				return (cleanup_cmds(&cmds, cleanincl), cmds);
			}
			cmdlnsecidx = 0;
			free(cmdln);
		}
		cmdln = cmdlnsec[cmdlnsecidx++];
		cmds = msh_compile_cmds_core(pcd, cmdln, cmds, sqed);
		ret = msh_check_cmds(pcd, cmds, cmdln, &cleanincl);
		if (ret == MSH_CMDS_OK)
			break;
		else if (ret == MSH_SYNTAX_ERR)
		{
			// return (clear_fdinput(pcd, STDIN_FILENO), cleanup_cmds(&cmds, cleanincl), cmds);
			clear_fdinput(pcd, STDIN_FILENO);
			cleanup_cmds(&cmds, NULL);
			if (pcd->interactive && cmdlnsec[cmdlnsecidx])
				continue ;
			pcd->exit = true;
		}
		else if (ret == MSH_UNFINISHED_PIPE)
		{
			pt = (t_pt)pt_contd;
			continue ;
		}
	}
	if (cmdlnsec && (!cmdlnsec[cmdlnsecidx] || pcd->exit))
	{
		rh_free_simple_array((void ***)&cmdlnsec);
	}
	return (cmds);
}

