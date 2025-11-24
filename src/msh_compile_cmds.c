/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_compile_cmds.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:12:30 by rhaas             #+#    #+#             */
/*   Updated: 2025/11/24 16:33:18 by rhaas            ###   ########.fr       */
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

static bool	msh_process_simplecmd_token(t_cd *pcd, t_tca *ptca,
	int *ptokenidx, t_sqed const sqed)
{
	int		res;
	char	**args;

	args = (char **) NULL;
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
				return (rh_free_simple_array((void ***)args), false);
			ptca->curarg->arg = args[res++];
		}
		free(args);
	}
	return (true);
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
		if (tca.tokens[tokenidx][0] != '#')
		{
			if (!(set_curcmd(&tca)
					&& msh_process_simplecmd_token(pcd, &tca, &tokenidx, sqed)))
				return (cleanup_tca(&tca), (t_cmd *) NULL);
		}
		++tokenidx;
		if (tca.curcmd && (tca.curcmd->ctrlchr || !tca.tokens[tokenidx]))
		{
			if (!convert_arglst(&tca))
				return (cleanup_tca(&tca), (t_cmd *) NULL);
			cleanup_arglst(&tca);
		}
	}
	return (cleanup_tca(&tca), tca.cmdlst);
}

bool	msh_compile_cmds_get_cmdlines(t_cd *pcd, char **pcmdln, t_cs cs)
{
	static t_cls	cls = (t_cls){.cmdlines = (char **) NULL, .idx = 0,};

	*pcmdln = (char *) NULL;
	if (!(cls.cmdlines && cls.cmdlines[cls.idx]))
	{
		cleanup_cls(&cls);
		if (!get_cmdline_add_history(pcd, pcmdln))
			return (cleanup_cls(&cls), false);
		else if (*pcmdln && *(*pcmdln) == 0)
			*pcmdln = "";
		else
			cls.cmdlines = rh_split_quotes(*pcmdln,
					rh_sqed2qes_cmdln(&pcd->default_sqed));
		cls.idx = 0;
	}
	if (cls.cmdlines && cls.cmdlines[cls.idx])
		*pcmdln = cls.cmdlines[cls.idx++];
	if (!(*pcmdln))
	{
		if (!pcd->interactive && cs == (t_cs)cs_unfinished_pipe)
			msh_syntaxerror(pcd, (char *) NULL, "unexpected end of file");
		return (cleanup_cls(&cls), false);
	}
	return (true);
}

static t_cmd	*msh_compile_multicmds(
	t_cd *pcd, t_sqed const sqed)
{
	char			*cmdln;
	t_cmd			*cmds;
	t_cs			cs;

	cmds = (t_cmd *) NULL;
	cs = (t_cs)cs_uninitialised;
	while (!pcd->exit)
	{
		if (!msh_compile_cmds_get_cmdlines(pcd, &cmdln, cs))
		{
			if (!pcd->interactive && cs == (t_cs)cs_unfinished_pipe)
				msh_syntaxerror(pcd, (char *) NULL, "unexpected end of file");
			pcd->exit = true;
			return (cleanup_cmds(&cmds), cmds);
		}
		cmds = msh_compile_cmds_core(pcd, cmdln, cmds, sqed);
		cs = msh_check_cmds(pcd, cmds, cmdln);
		if (msh_check_cmdstatus_ok(pcd, cs, &cmds))
			break ;
	}
	return (cmds);
}

t_cmd	*msh_compile_cmds(t_cd *pcd, t_sqed const sqed)
{
	static t_cmd	*cmds = (t_cmd *) NULL;
	t_cmd			*subcmds;
	t_cmd			*subcmds_last;

	if (!cmds)
		cmds = msh_compile_multicmds(pcd, sqed);
	if (!cmds)
		return ((t_cmd *) NULL);
	subcmds = cmds;
	while (cmds->ctrlchr == '|')
		cmds = cmds->next;
	subcmds_last = cmds;
	cmds = cmds->next;
	subcmds_last->next = (t_cmd *) NULL;
	return (subcmds);
}
