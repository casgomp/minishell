/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rh_rmquotes_esc_expnd2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 16:38:01 by rhaas             #+#    #+#             */
/*   Updated: 2025/10/22 12:11:10 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
// #include "rh_split_qed.h"
#include "rh_split_qed_intern.h"

#include <stdbool.h>
#include <stdlib.h>

int	set_tsstl(t_uedata *pue, char *const pcstart, size_t len)
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
	*(pue->tsl) = (t_strstart_len){
		.newstr = (pue->tsl == pue->tsl_orig), .pcstart = pcstart, .len = len,
		.next = (t_strstart_len*) NULL};
	return (1);
}

// returns...
//  1 if escape did happen
//  0 if not
// -1 on invalid input or strlcpy error
int	escape(char *const pc, int *const pidx, t_uedata *const puedata)
{
	size_t	pclength;

	if (!(pc && puedata))
		return (-1);
	if ((!puedata->actvq) || (puedata->actvq == *(pc + 1)))
	{
		pclength = ft_strlen(pc);
		if (ft_strlcpy(pc, pc + 1, pclength) != (pclength - 1))
			return (-1);
		++(*pidx);
		++(puedata->tsl->len);
		return (1);
	}
	return (0);
}

int	expand_value(char *const envval, char ***pwordsplit,
	t_qed const *const pqed, t_uedata *const puedata)
{
	int		idx;

	if (!puedata->actvq)
	{
		*pwordsplit = rh_split_quotes(envval, (t_qes){.blank = pqed->ifs,
				.esc = '\0', .metachr = "", .q = ""});
		if (!(*pwordsplit))
			return (false);
		idx = 0;
		if (!(*pwordsplit)[idx] && puedata->tsl->len == 0)
		{
			if (!set_tsstl(puedata, (char *) NULL, 0))
				return (rh_free_simple_array((void **)(*pwordsplit)), false);
			// puedata->tsl->newstr = false;
			// puedata->tsl->pcstart = (char *) NULL;
		}
		while ((*pwordsplit)[idx])
		{
			if (!set_tsstl(puedata, (*pwordsplit)[idx], ft_strlen((*pwordsplit)[idx])))
				return (rh_free_simple_array((void **)(*pwordsplit)), false);
			puedata->tsl->newstr |= (idx > 0);
			puedata->uestrlen += puedata->tsl->len;
			++idx;
		}
		free(*pwordsplit);
	}
	else if (!set_tsstl(puedata, envval, ft_strlen(envval)))
		return (false);
	return (true);
}

// returns...
//  2 if expansion did happen
//  0 if expansion did not happen
// -2 if set_tsstl failed after expansion happened
// internal hint: paramlen starts at 1, bc the expnd char must also be counted
int	expand(char *const pc, int *const pidx,
	t_qed const *const pqed, t_uedata *const puedata)
{
	t_ews	ews;

	ews = (t_ews){.paramlen = 1, .isfirstchar = true, .wordsplit = NULL};
	while (/* pc[ews.paramlen]
		&& */ (*pqed->validchar)(pc[ews.paramlen], &ews.isfirstchar))
		++ews.paramlen;
	if (ews.paramlen > 1)
	{
		ews.envvar = ft_substr(pc, 1, ews.paramlen - 1);
		if (ews.envvar)
		{
			ews.envval = (*pqed->dict)(ews.envvar, pqed->dict_data);
			free(ews.envvar);
			if (ews.envval
				&& !expand_value(ews.envval, &ews.wordsplit, pqed, puedata))
				return (/* free(ews.wordsplit),  */-2);
		}
		*pidx += ews.paramlen;
		if (puedata->instr[*pidx]/*  && puedata->tsl->pcstart */
			&& !set_tsstl(puedata, &(puedata->instr[*pidx]), 0))
			return (/* free(ews.wordsplit), */ -2);
		return (2);
	}
	return (0);
}

// returns the return values of escape resp. expand
int	escape_expand(char *const pc, int *const pidx,
				t_qed const *const pqed, t_uedata *const puedata)
{
	if (puedata->actvq == pqed->q[0] || !puedata->actvq)
	{
		if (pqed->esc == *pc)
		{
			return (escape(pc, pidx, puedata));
		}
		else if (pqed->expnd == *pc)
		{
			return (expand(pc, pidx, pqed, puedata));
		}
	}
	return (0);
}
