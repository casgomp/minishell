/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_quotes2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 17:57:26 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/18 11:45:44 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "rh_split_quotes_intern.h"

#include <stdlib.h>

void	cleanup(t_splprms *const ptsp)
{
	size_t		idx;
	t_startlen	*tmp;

	idx = 0;
	while (ptsp->split && ptsp->split[idx])
		free(ptsp->split[idx++]);
	free(ptsp->split);
	ptsp->split = (char **) NULL;
	while (ptsp->tsl_orig)
	{
		tmp = ptsp->tsl_orig->next;
		free(ptsp->tsl_orig);
		ptsp->tsl_orig = tmp;
	}
	ptsp->tsl_orig = (t_startlen *) NULL;
}

int	set_tsl(t_splprms *const ptsp, int idx)
{
	if (!(ptsp->tsl_orig) || (ptsp->tsl)->len)
	{
		if (!(ptsp->tsl_orig))
		{
			ptsp->tsl_orig = malloc(sizeof(t_startlen));
			ptsp->tsl = ptsp->tsl_orig;
		}
		else
		{
			ptsp->tsl->next = malloc(sizeof(t_startlen));
			ptsp->tsl = ptsp->tsl->next;
		}
		*(ptsp->tsl) = (t_startlen){idx, 0, (t_startlen *) NULL};
		++(ptsp->n_slelem);
	}
	return (ptsp->tsl != (t_startlen *) NULL);
}

static int	transform_prep(t_splprms *const ptsp)
{
	if (ptsp->activeq)
		return (0);
	ptsp->split = (char **)malloc((ptsp->n_slelem + 1) * sizeof(char *));
	if (!ptsp->split)
		return (0);
	ptsp->split[ptsp->n_slelem] = (char *) NULL;
	ptsp->tsl = ptsp->tsl_orig;
	return (1);
}

char	**transform_list(t_splprms *const ptsp, char const *const s)
{
	int		idx;
	char	**splt;

	if (!transform_prep(ptsp))
		return (cleanup(ptsp), (char **) NULL);
	splt = ptsp->split;
	idx = 0;
	while (ptsp->tsl)
	{
		splt[idx] = ft_substr(s, ptsp->tsl->start, ptsp->tsl->len);
		if (!splt[idx])
			return (cleanup(ptsp), (char **) NULL);
		++idx;
		ptsp->tsl = ptsp->tsl->next;
	}
	ptsp->split = (char **) NULL;
	return (cleanup(ptsp), splt);
}

// static int	preproc_qesstr(char const *const s, int *const pidx,
// 									t_qes const *const pqes, t_splprms *ptsp)
// {
// 	if (s[*pidx] && !ptsp->activeq)
// 	{
// 		if (ft_strchr(pqes->q, s[*pidx]))
// 			ptsp->activeq = s[(*pidx)++];
// 		else if (ft_strchr(pqes->specialsplt, s[*pidx]))
// 		{
// 			while (s[*pidx + ptsp->tsl->len] == s[*pidx])
// 				++(ptsp->tsl->len);
// 			*pidx += ptsp->tsl->len;
// 			if (s[*pidx] && !set_tsl(ptsp, *pidx))
// 				return (0);
// 		}
// 	}
// 	return (1);
// }

// static void	postproc_qesstr(char const *const s, int *const pidx,
// 									t_qes const *const pqes, t_splprms *ptsp)
// {
// 	if (ptsp->activeq && s[*pidx] == ptsp->activeq)
// 	{
// 		ptsp->activeq = '\0';
// 		++(*pidx);
// 	}
// 	if (!s[*pidx]
// 		|| (!ptsp->activeq
// 			&& (s[*pidx] == pqes->splt
// 				|| ft_strchr(pqes->specialsplt, s[*pidx]))))
// 		ptsp->tsl->len = *pidx - ptsp->tsl->start;
// }

// static void	advance_qesstr(char const *const s, int *const pidx,
// 									t_qes const *const pqes, t_splprms *ptsp)
// {
// 	while (s[*pidx]
// 		&& ((!ptsp->activeq && !ft_strchr(pqes->q, s[*pidx])
// 				&& (s[*pidx] != pqes->splt)
// 				&& !ft_strchr(pqes->specialsplt, s[*pidx]))
// 			|| (ptsp->activeq && s[*pidx] != ptsp->activeq)))
// 	{
// 		if (s[*pidx] == pqes->esc && s[*pidx] == pqes->q[0])
// 			++(*pidx);
// 		++(*pidx);
// 	}
// }

// // Same as ft_split, but takes a string containing quote chars
// // (typically "\\"\\'") that will cancel the effect of the split char
// // in the section between the quote chars
// char	**ft_split_quotes(char const *const s, t_qes const qes)
// {
// 	t_splprms	tsp;
// 	int			idx;

// 	tsp = (t_splprms){(char **) NULL, (t_startlen *) NULL, NULL, 0, '\0'};
// 	if (!(s && qes.splt))
// 		return (cleanup(&tsp), (char **) NULL);
// 	idx = 0;
// 	while (s[idx])
// 	{
// 		while (s[idx] && qes.splt == s[idx])
// 			++idx;
// 		if (s[idx] && !set_tsl(&tsp, idx))
// 			return (cleanup(&tsp), (char **) NULL);
// 		if (!preproc_qesstr(s, &idx, &qes, &tsp))
// 			return (cleanup(&tsp), (char **) NULL);
// 		advance_qesstr(s, &idx, &qes, &tsp);
// 		postproc_qesstr(s, &idx, &qes, &tsp);
// 	}
// 	return (transform_list(&tsp, s));
// }
