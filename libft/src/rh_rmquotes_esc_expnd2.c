/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rh_rmquotes_esc_expnd2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 16:38:01 by rhaas             #+#    #+#             */
/*   Updated: 2025/10/23 12:59:26 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "rh_split_qed_intern.h"

#include <stdbool.h>
#include <stdlib.h>

int	set_tsstl(t_uedata *pue, char *const pcstart, size_t len, bool expanded)
{
	t_strstart_len	*new_tsstl;

	if (!(pue->tsl_orig) || (pue->tsl)->len)
	{
		new_tsstl = malloc(sizeof(t_strstart_len));
		if (!new_tsstl)
			return (0);
		if (!(pue->tsl_orig))
		{
			pue->tsl_orig = new_tsstl;
			pue->tsl = pue->tsl_orig;
		}
		else
		{
			pue->uestrlen += pue->tsl->len;
			pue->tsl->next = new_tsstl;
			pue->tsl = pue->tsl->next;
		}
	}
	*(pue->tsl) = (t_strstart_len){.newstr = (pue->tsl == pue->tsl_orig),
		.expanded = expanded, .pcstart = pcstart, .len = len,
		.next = (t_strstart_len*) NULL};
	return (1);
}

// returns...
//  1 if escape did happen
//  0 if not
// -1 on invalid input or strlcpy error
int	escape(char *const pc, int *const pidx, t_qed const *const pqed,
	t_uedata *const puedata)
{
	if (!(pc && puedata))
		return (-1);
	if ((!puedata->actvq)
		|| (puedata->actvq == pqed->q[0] && puedata->actvq == *(pc + 1)))
	{
		if (pqed->rmqe)
			ft_strlcpy(pc, pc + 1, ft_strlen(pc));
		else
		{
			++(*pidx);
			++(puedata->tsl->len);
		}
		++(*pidx);
		++(puedata->tsl->len);
		return (1);
	}
	return (0);
}

static int	expand_value(char *const envval, t_uedata *const puedata)
{
	if (!set_tsstl(puedata, envval, ft_strlen(envval), true))
		return (false);
	return (true);
}

// returns...
//  2 if expansion did happen
//  0 if expansion did not happen
// -2 if set_tsstl failed after expansion happened
// internal hint: paramlen starts at 1, bc the expnd char must also be counted
int	expand(char *const pc, int *const pidx,
	t_qed *const pqed, t_uedata *const puedata)
{
	t_ews	ews;

	ews = (t_ews){.paramlen = 1, .isfirstchar = true};
	while ((*pqed->validchar)(pc[ews.paramlen], &ews.isfirstchar))
		++ews.paramlen;
	if (ews.paramlen > 1)
	{
		ews.envvar = ft_substr(pc, 1, ews.paramlen - 1);
		if (ews.envvar)
		{
			ews.envval = (*pqed->dict)(ews.envvar, pqed->dict_data);
			free(ews.envvar);
			if (ews.envval
				&& !expand_value(ews.envval, puedata))
				return (-2);
		}
		*pidx += ews.paramlen;
		if (puedata->instr[*pidx]
			&& !set_tsstl(puedata, &(puedata->instr[*pidx]), 0, false))
			return (-2);
		pqed->expstat = true;
		return (2);
	}
	return (0);
}

// returns the return values of escape resp. expand
int	escape_expand(char *const pc, int *const pidx,
				t_qed *const pqed, t_uedata *const puedata)
{
	if (*pc
		&& (puedata->actvq == pqed->q[0] || !puedata->actvq))
	{
		if (pqed->esc == *pc)
		{
			return (escape(pc, pidx, pqed, puedata));
		}
		else if (pqed->expnd == *pc)
		{
			return (expand(pc, pidx, pqed, puedata));
		}
	}
	return (0);
}
