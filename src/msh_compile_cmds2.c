/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_compile_cmds2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:12:30 by rhaas             #+#    #+#             */
/*   Updated: 2025/11/17 20:55:14 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "msh_builtins.h"
#include "msh_compile_cmds.h"
#include "msh_compile_cmds_intern.h"
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

int	msh_process_metachar(t_cd *pcd, t_tca *ptca,
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

bool	msh_check_cmdstatus_ok(t_cd *const pcd, int ret, t_cmd **pcmds)
{
	if (!pcd->exit && ret == (t_cs)cs_cmds_ok)
		return (true);
	else if ((ret == (t_cs)cs_syntax_err) && (!(pcd->interactive)))
		pcd->exit = true;
	else if (ret == (t_cs)cs_unfinished_pipe)
		pcd->pt = (t_pt)pt_contd;
	if (pcd->exit)
		cleanup_cmds(pcmds);
	return (false);
}

t_cs	msh_check_cmds(t_cd *const pcd, t_cmd const *const cmds,
	char const *const cmdline)
{
	t_cmd const	*cmd = cmds;
	char		buf[64];

	while (cmd)
	{
		if (ft_strchr("|;", cmd->ctrlchr))
		{
			if (!(cmd->args && cmd->args[0]))
			{
				ft_sprintf(buf, "%c", cmd->ctrlchr);
				msh_syntaxerror(pcd, buf, cmdline);
				return ((t_cs)cs_syntax_err);
			}
			if (cmd->ctrlchr == '|' && !cmd->next)
			{
				return ((t_cs)cs_unfinished_pipe);
			}
		}
		cmd = cmd->next;
	}
	return ((t_cs)cs_cmds_ok);
}
