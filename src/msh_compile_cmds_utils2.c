/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_compile_cmds_utils2.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:12:30 by rhaas             #+#    #+#             */
/*   Updated: 2025/11/13 10:51:18 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_compile_cmds_intern.h"

#include <stdlib.h>

int	convert_arglst(t_tca *ptca)
{
	int	argidx;

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
