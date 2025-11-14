/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_compile_cmds.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:12:30 by rhaas             #+#    #+#             */
/*   Updated: 2025/11/14 16:59:36 by rhaas            ###   ########.fr       */
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

static bool	msh_compile_cmds_checkcmmt(t_tca *ptca, int tokenidx)
{
	int		tkidx;
	int		offset;
	char	*nln;

	tkidx = tokenidx;
	nln = (char *) NULL;
	if (ptca->tokens[tokenidx][0] == '#')
	{
		while (ptca->tokens[tkidx] && !nln)
		{
			nln = ft_strchr(ptca->tokens[tkidx], '\n');
			if (nln)
				nln = ft_strdup(nln);
			free(ptca->tokens[tkidx]);
			ptca->tokens[tkidx++] = (char *) NULL;
		}
		if (nln)
		{
			ptca->tokens[--tkidx] = nln;
			offset = tkidx;
			while (offset > 0 && ptca->tokens[tkidx++])
				ptca->tokens[tkidx - 1 - offset] = ptca->tokens[tkidx - 1];
		}
		// else
			return (true);
	}
	return (false);
}

static t_cmd	*msh_compile_cmds_core(t_cd *pcd, char *pcmdline,
			t_cmd *cmdlst_sofar, t_sqed const sqed)
{
	t_tca		tca;
	int			tokenidx;

	tca = (t_tca){.tosplit = pcmdline, .cmdlst = cmdlst_sofar,
		.curcmd = (t_cmd *) NULL, .arglst = (t_cmdarg *) NULL,
		.tokens = rh_split_quotes(pcmdline, rh_sqed2qes(&sqed))};
	tokenidx = 0;
	while (tca.tokens && (*tca.tokens) && tca.tokens[tokenidx])
	{
		if (!msh_compile_cmds_checkcmmt(&tca, tokenidx))
		{
			// if (tca.tokens[tokenidx][0] == '\n')
			// {
			// 	free (*pcmdline);
			// 	*pcmdline = ft_strdup(&tca.tokens[tokenidx][1]);
			// 	// tca.tosplit = *pcmdline;
			// 	// rh_free_simple_array((void ***)&tca.tokens);
			// 	// tca.tokens = rh_split_quotes(*pcmdline, rh_sqed2qes(&sqed));
			// }
			if (!(set_curcmd(&tca)
					&& msh_process_simplecmd_token(pcd, &tca, &tokenidx, sqed)))
				return (cleanup_tca(&tca), (t_cmd *) NULL);
			++tokenidx;
		}
		if (tca.curcmd && (tca.curcmd->ctrlchr || !tca.tokens[tokenidx]))
		{
			if (!convert_arglst(&tca))
				return (cleanup_tca(&tca), (t_cmd *) NULL);
			cleanup_arglst(&tca);
		}
	}
	return (cleanup_tca(&tca), tca.cmdlst);
}

static void	cleanup_iolst(t_io **piolst_head)
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

static void	cleanup_cmds(t_cmd **pcmds_head/* , t_cmd const *clean_incl */)
{
	t_cmd	*cmd;
	// int		stop_cleanup;
	bool	stop_clean;

	stop_clean = false;
	if (!pcmds_head)
		return ;
	while (*pcmds_head && !stop_clean /* stop_cleanup < 2 */)
	{
		rh_free_simple_array((void ***)(&((*pcmds_head)->args)));
		cleanup_iolst(&((*pcmds_head)->iolst));
		// stop_cleanup += ((*pcmds_head) == clean_incl);
		// stop_cleanup += (stop_cleanup && ((*pcmds_head)->ctrlchr == '\n'));
		stop_clean = ((*pcmds_head)->ctrlchr == '\n');
		cmd = (*pcmds_head)->next;
		free(*pcmds_head);
		(*pcmds_head) = cmd;
	}
}

static bool	msh_check_cmds_status(t_cd *const pcd, int ret, t_pt *ppt,
		t_cmd **pcmds)
{
	if (ret == MSH_CMDS_OK)
		return (true);
	else if (ret == MSH_SYNTAX_ERR)
	{
		cleanup_cmds(pcmds);
		if (!(pcd->interactive))
		{
			clear_fdinput(STDIN_FILENO);			
			pcd->exit = true;
		}
	}
	else if (ret == MSH_UNFINISHED_PIPE)
	{
		*ppt = (t_pt)pt_contd;
	}
	return (false);
}

static int	msh_check_cmds(t_cd *const pcd, t_cmd const *const cmds,
	char const *const cmdline/* , t_cmd const **cleanincl */)
{
	t_cmd const	*cmd = cmds;

	while (cmd)
	{
		if (cmd->ctrlchr == '|')
		{
			if (!(cmd->args && cmd->args[0]))
			{
				// *cleanincl = cmd;
				return (msh_syntaxerror(pcd, "|", cmdline), MSH_SYNTAX_ERR);
			}
			if (!cmd->next)
			{
				return (MSH_UNFINISHED_PIPE);
			}
		}
		cmd = cmd->next;
	}
	return (MSH_CMDS_OK);
}

static bool	get_cmdlnsections(t_cd *pcd, t_pt pt,
		char ***const pcmdlnsec, int *const pcmdlnsecidx)
{
	char	*cmdln;

	if (!((*pcmdlnsec) && (*pcmdlnsec)[(*pcmdlnsecidx)]))
	{
		if (!get_cmdline_add_history(pcd, &cmdln, pt))
			return (false);
		(*pcmdlnsec) = rh_split_quotes(cmdln,
				rh_sqed2qes_cmdln(&pcd->default_sqed));
		if (!((*pcmdlnsec) && (*pcmdlnsec)[0]))
		{
			rh_free_simple_array((void ***)pcmdlnsec);
			return (false);
		}
		(*pcmdlnsecidx) = 0;
		free(cmdln);
	}
	return (true);
}

t_cmd	*msh_compile_cmds(t_cd *pcd, t_sqed const sqed)
{
	t_cmd		*cmds;
	char		*cmdln;
	static char	**cmdlnsec = (char **) NULL;
	static int	cmdlnsecidx = 0;
	int			ret;
	t_pt		pt;

	cmdln = (char *) NULL;
	cmds = (t_cmd *) NULL;
	pt = (t_pt)pt_std;
	while (!pcd->exit)
	{
		if (!get_cmdlnsections(pcd, pt, &cmdlnsec, &cmdlnsecidx))
		{
			if (!pcd->interactive && ret == MSH_UNFINISHED_PIPE)
				msh_syntaxerror(pcd, (char *) NULL, "unexpected end of file");
			return (cleanup_cmds(&cmds), cmds);
		}
		cmdln = cmdlnsec[cmdlnsecidx++];
		cmds = msh_compile_cmds_core(pcd, cmdln, cmds, sqed);
		ret = msh_check_cmds(pcd, cmds, cmdln);
		if (msh_check_cmds_status(pcd, ret, &pt, &cmds))
			break;
	}
	if (cmdlnsec && (!cmdlnsec[cmdlnsecidx] || pcd->exit))
		rh_free_simple_array((void ***)&cmdlnsec);
	return (cmds);
}
