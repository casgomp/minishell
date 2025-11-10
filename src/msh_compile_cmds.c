/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_compile_cmds.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:12:30 by rhaas             #+#    #+#             */
/*   Updated: 2025/11/10 16:54:23 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "msh_builtins.h"
#include "msh_compile_cmds.h"
#include "msh_compile_cmds_intern.h"

#include <limits.h>
#include <stdlib.h>

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
	else if (ft_strchr("<>", token[tokenlen - 1]))
	{
		if (msh_process_ioredirector(pcd, ptca, tokenidx, sqed))
			++(*ptokenidx);
		else
			return (-1);
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
			t_sqed const sqed)
{
	t_tca		tca;
	int			tokenidx;

	tca.tosplit = cmdline;
	tca.tokens = rh_split_quotes(cmdline, rh_sqed2qes(&sqed));
	tca.cmdlst = (t_cmd *) NULL;
	tca.arglst = (t_cmdarg *) NULL;
	tokenidx = 0;
	while (tca.tokens && tca.tokens[tokenidx])
	{
		if (!set_curcmd(&tca))
			return (cleanup_tca(&tca), NULL);
		if (!msh_process_simplecmd_token(pcd, &tca, &tokenidx, sqed))
			return (cleanup_tca(&tca), (t_cmd *) NULL);
		++tokenidx;
		if (tca.curcmd->ctrlchr || !tca.tokens[tokenidx])
		{
			if (!convert_arglst(&tca))
				return (cleanup_tca(&tca), (t_cmd *) NULL);
			cleanup_arglst(&tca);
		}
	}
	return (cleanup_tca(&tca), tca.cmdlst);
}

static bool	msh_check_cmds(t_cd *const pcd, t_cmd const *const cmds,
	char const *const cmdline)
{
	t_cmd const	*cmd = cmds;
	while (cmd)
	{
		if (cmd->ctrlchr == '|')
		{
			if (!(cmd->args && cmd->args[0]))
			{
				msh_syntaxerror(pcd, "|", cmdline);
				return false;
			}
			// // Possibly handle this case differently and
			// // continue reading from prompt until msh_check_cmds 
			// // is happy?
			if (!cmd->next)
			{
				msh_syntaxerror(pcd, "newline", cmdline);
				return false;
			}
		}
		cmd = cmd->next;
	}
	return (true);
}

t_cmd	*msh_compile_cmds(t_cd *pcd, char const *const cmdline,
			t_sqed const sqed)
{
	t_cmd	*const cmds = msh_compile_cmds_core(pcd, cmdline,sqed);

	if (!msh_check_cmds(pcd, cmds, cmdline))
	{
		// ...cleanup cmdlst == delete all cmds
		//    nothing should be executed
		;
	}
	return (cmds);
}

