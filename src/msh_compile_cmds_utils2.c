/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_compile_cmds_utils2.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:12:30 by rhaas             #+#    #+#             */
/*   Updated: 2025/11/21 14:39:09 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_compile_cmds_intern.h"

#include <stdlib.h>

int	cleanup_arglst(t_tca *ptca)
{
	while (ptca->arglst)
	{
		ptca->curarg = ptca->arglst->next;
		if (ptca->num_args)
			free(ptca->arglst->arg);
		free(ptca->arglst);
		ptca->arglst = ptca->curarg;
	}
	ptca->num_args = 0;
	return (1);
}

int	cleanup_tca(t_tca *ptca)
{
	rh_free_simple_array((void ***)&(ptca->tokens));
	ptca->tokens = (char **) NULL;
	if (!cleanup_arglst(ptca))
		return (0);
	return (1);
}

void	cleanup_iolst(t_io **piolst_head)
{
	t_io	*elem;

	if (!piolst_head)
		return ;
	while (*piolst_head)
	{
		elem = (*piolst_head)->next;
		free ((*piolst_head)->fname_delim);
		free(*piolst_head);
		(*piolst_head) = elem;
	}
	(*piolst_head) = (t_io *) NULL;
}

void	cleanup_cls(t_cls *pcls)
{
	rh_free_simple_array((void ***)&(pcls->cmdlines));
}

void	cleanup_cmds(t_cmd **pcmds_head)
{
	t_cmd	*cmd;

	if (!pcmds_head)
		return ;
	while (*pcmds_head)
	{
		rh_free_simple_array((void ***)(&((*pcmds_head)->args)));
		cleanup_iolst(&((*pcmds_head)->iolst));
		cmd = (*pcmds_head)->next;
		free(*pcmds_head);
		(*pcmds_head) = cmd;
	}
}
