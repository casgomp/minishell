/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rh_split_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 17:57:26 by rhaas             #+#    #+#             */
/*   Updated: 2025/11/15 23:17:59 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "rh_split_qed_intern.h"

#include <limits.h>
#include <stdlib.h>

static int	preproc_qesstr(char const *const s, int *const pidx,
									t_qes const *const pqes, t_splprms *ptsp)
{
	if (s[*pidx]
		&& !ptsp->activeq
		&& ft_strchr(pqes->q, s[*pidx]))
	{
		ptsp->activeq = s[(*pidx)++];
	}
	return (1);
}

static void	advance_qesstr(char const *const s, int *const pidx,
									t_qes const *const pqes, t_splprms *ptsp)
{
	while (s[*pidx]
		&& ((!ptsp->activeq
				&& !ft_strchr(pqes->q, s[*pidx])
				&& !(pqes->lncmt == s[*pidx])
				&& !ft_strchr(pqes->blank, s[*pidx])
				&& !ft_strchr(pqes->metachr, s[*pidx]))
			|| (ptsp->activeq && s[*pidx] != ptsp->activeq)))
	{
		if ((s[*pidx] == pqes->esc)
			&& (!ptsp->activeq
				|| (ptsp->activeq == pqes->q[0] && s[*pidx + 1] == pqes->q[0])))
		{
			++(*pidx);
		}
		++(*pidx);
	}
}

static void	postproc_metachars(char const *const s, int *const pidx,
								t_splprms *ptsp)
{
	char const	metac = s[*pidx];

	if ((ptsp->tsl->len == 0) || (metac == '>'))
	{
		while (s[ptsp->tsl->start + ptsp->tsl->len] == metac)
			++(ptsp->tsl->len);
		*pidx = ptsp->tsl->start + ptsp->tsl->len;
	}
}

static void	postproc_qesstr(char const *const s, int *const pidx,
									t_qes const *const pqes, t_splprms *ptsp)
{
	if (ptsp->activeq && s[*pidx] == ptsp->activeq)
	{
		ptsp->activeq = '\0';
		++(*pidx);
	}
	if (ptsp->tsl
		&& (((!s[*pidx]) && (!ptsp->tsl->len))
			|| (s[*pidx]
				&& (!ptsp->activeq)
				&& (ft_strchr(pqes->blank, s[*pidx])
					|| ft_strchr(pqes->metachr, s[*pidx])))))
	{
		ptsp->tsl->len = *pidx - ptsp->tsl->start;
		if (s[*pidx] && ft_strchr(pqes->metachr, s[*pidx]))
			postproc_metachars(s, pidx, ptsp);
	}
}
// ^^^^^^^^ Optionally ^^^^^^^^
// ... if postproc_metachars is passed as a fnctn ptr:
// if (postproc_metachars
// 	   && s[*pidx] && ft_strchr(pqes->metachr, s[*pidx]))
// 	postproc_metachars(s, pidx, ptsp);

// Same as ft_split, but takes a string containing quote chars
// (typically "\\"\\'") that will cancel the effect of the split char
// in the section between the quote chars
char	**rh_split_quotes(char const *const s, t_qes const qes)
{
	t_splprms	tsp;
	int			idx;

	tsp = (t_splprms){(char **) NULL, (t_startlen *) NULL, NULL, 0, '\0'};
	if (!(s && qes.blank))
		return (cleanup_splt(&tsp), (char **) NULL);
	idx = 0;
	while (s[idx])
	{
		while (s[idx] && ft_strchr(qes.blank, s[idx]))
			++idx;
		if (s[idx] && !set_tsl(&tsp, idx))
			return (cleanup_splt(&tsp), (char **) NULL);
		if (s[idx] == qes.lncmt)
		{
			while (s[idx] && s[idx] != '\n')
				++idx;
		}
		if (preproc_qesstr(s, &idx, &qes, &tsp) == 2)
			continue ;
		advance_qesstr(s, &idx, &qes, &tsp);
		postproc_qesstr(s, &idx, &qes, &tsp);
	}
	return (transform_list(&tsp, s));
}
