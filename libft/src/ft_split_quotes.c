/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raphha <raphha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 17:57:26 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/21 13:05:41 by raphha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "rh_split_quotes_intern.h"

#include <stdlib.h>

static int	preproc_qesstr(char const *const s, int *const pidx,
									t_qes const *const pqes, t_splprms *ptsp)
{
	if (s[*pidx] && !ptsp->activeq)
	{
		if (ft_strchr(pqes->q, s[*pidx]))
			ptsp->activeq = s[(*pidx)++];
		else if (ft_strchr(pqes->specialsplt, s[*pidx]))
		{
			while (s[*pidx + ptsp->tsl->len] == s[*pidx])
				++(ptsp->tsl->len);
			*pidx += ptsp->tsl->len;
			return (2);
		}
	}
	return (1);
}

static void	postproc_qesstr(char const *const s, int *const pidx,
									t_qes const *const pqes, t_splprms *ptsp)
{
	if (ptsp->activeq && s[*pidx] == ptsp->activeq)
	{
		ptsp->activeq = '\0';
		++(*pidx);
	}
	if (!s[*pidx]
		|| (!ptsp->activeq
			&& (s[*pidx] == pqes->splt
				|| ft_strchr(pqes->specialsplt, s[*pidx]))))
		ptsp->tsl->len = *pidx - ptsp->tsl->start;
}

static void	advance_qesstr(char const *const s, int *const pidx,
									t_qes const *const pqes, t_splprms *ptsp)
{
	while (s[*pidx]
		&& ((!ptsp->activeq && !ft_strchr(pqes->q, s[*pidx])
				&& (s[*pidx] != pqes->splt)
				&& !ft_strchr(pqes->specialsplt, s[*pidx]))
			|| (ptsp->activeq && s[*pidx] != ptsp->activeq)))
	{
		if (s[*pidx] == pqes->esc && s[*pidx] == pqes->q[0])
			++(*pidx);
		++(*pidx);
	}
}

// Same as ft_split, but takes a string containing quote chars
// (typically "\\"\\'") that will cancel the effect of the split char
// in the section between the quote chars
char	**ft_split_quotes(char const *const s, t_qes const qes)
{
	t_splprms	tsp;
	int			idx;

	tsp = (t_splprms){(char **) NULL, (t_startlen *) NULL, NULL, 0, '\0'};
	if (!(s && qes.splt))
		return (cleanup(&tsp), (char **) NULL);
	idx = 0;
	while (s[idx])
	{
		while (s[idx] && qes.splt == s[idx])
			++idx;
		if (s[idx] && !set_tsl(&tsp, idx))
			return (cleanup(&tsp), (char **) NULL);
		if (preproc_qesstr(s, &idx, &qes, &tsp) == 2)
			continue ;
		advance_qesstr(s, &idx, &qes, &tsp);
		postproc_qesstr(s, &idx, &qes, &tsp);
	}
	return (transform_list(&tsp, s));
}
