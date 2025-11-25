/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_compile_cmds_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:12:30 by rhaas             #+#    #+#             */
/*   Updated: 2025/11/17 20:42:29 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "msh_compile_cmds_intern.h"

#include <stdlib.h>

int	set_curcmd(t_tca *ptca)
{
	t_cmd	*newcmd;

	ptca->curcmd = ptca->cmdlst;
	while (ptca->curcmd && ptca->curcmd->next)
		ptca->curcmd = ptca->curcmd->next;
	if (!ptca->cmdlst || ptca->curcmd->ctrlchr)
	{
		newcmd = (t_cmd *)malloc(sizeof(t_cmd));
		if (!newcmd)
			return (0);
		if (!ptca->cmdlst)
		{
			ptca->cmdlst = newcmd;
			ptca->curcmd = ptca->cmdlst;
		}
		else
		{
			ptca->curcmd->next = newcmd;
			ptca->curcmd = ptca->curcmd->next;
		}
		*(ptca->curcmd) = (t_cmd){.iolst = (t_io *) NULL,
			.args = (char **) NULL, .ctrlchr = '\0', .next = (t_cmd *) NULL};
		ptca->num_args = 0;
	}
	return (ptca->curcmd != (t_cmd *) NULL);
}

int	get_iolstelem(t_tca *ptca)
{
	t_io *const	newio = malloc(sizeof(t_io));

	if (!newio)
		return (0);
	if (!ptca->curcmd->iolst)
	{
		ptca->curcmd->iolst = newio;
		ptca->curcmd->curio = ptca->curcmd->iolst;
	}
	else
	{
		ptca->curcmd->curio->next = newio;
		ptca->curcmd->curio = ptca->curcmd->curio->next;
	}
	*(ptca->curcmd->curio) = (t_io){.fname_delim = (char *) NULL, .new_fd = -1,
		.old_fd = -1, .tredirect = (t_rd)rd_default, .next = (t_io *) NULL};
	return (1);
}

int	get_curarg(t_tca *ptca)
{
	t_cmdarg *const	newcmdarg = malloc(sizeof(t_cmdarg));

	if (!newcmdarg)
		return (0);
	if (!ptca->arglst)
	{
		ptca->arglst = newcmdarg;
		ptca->curarg = ptca->arglst;
	}
	else
	{
		ptca->curarg->next = newcmdarg;
		ptca->curarg = ptca->curarg->next;
	}
	*(ptca->curarg) = (t_cmdarg){
		.arg = (char *) NULL, .next = (t_cmdarg *) NULL};
	++(ptca->num_args);
	return (1);
}

int	convert_arglst(t_tca *ptca)
{
	int	argidx;

	if (ptca->curcmd->args && !ptca->arglst)
		return (1);
	ptca->curcmd->args = malloc((ptca->num_args + 1) * sizeof(char *));
	if (!ptca->curcmd->args)
		return (0);
	ptca->curcmd->args[ptca->num_args] = (char *) NULL;
	argidx = -1;
	ptca->curarg = ptca->arglst;
	while (ptca->num_args)
	{
		ptca->curcmd->args[++argidx] = ptca->curarg->arg;
		ptca->curarg = ptca->curarg->next;
		--ptca->num_args;
	}
	return (1);
}
