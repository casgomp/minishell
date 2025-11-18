/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rh_split_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 17:57:26 by rhaas             #+#    #+#             */
/*   Updated: 2025/11/18 14:14:49 by rhaas            ###   ########.fr       */
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
				&& !(pqes->lncmt[0] == s[*pidx])
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
		ptsp->tsl->complete = true;
		ptsp->tsl->len = *pidx - ptsp->tsl->start;
		if (s[*pidx] && ft_strchr(pqes->metachr, s[*pidx]))
			postproc_metachars(s, pidx, ptsp);
	}
}

void	advance_comment(char const *const s, int *const pidx, t_qes const qes)
{
	if (s[(*pidx)] == qes.lncmt[0])
	{
		if ((*pidx) == 0
			|| ft_strchr(&qes.lncmt[1], s[(*pidx) - 1])
			|| ft_strchr(qes.metachr, s[(*pidx) - 1]))
		{
			while (s[(*pidx)] && s[(*pidx)] != '\n')
				++(*pidx);
		}
		else
			++(*pidx);
	}
}

char	**rh_split_quotes(char const *const s, t_qes const qes)
{
	t_splprms	tsp;
	int			idx;

	tsp = (t_splprms){.split = (char **) NULL, .tsl_orig = (t_startlen *) NULL,
		.tsl = (t_startlen *) NULL, .n_slelem = 0, .activeq = '\0'};
	if (!(s && qes.blank))
		return (cleanup_tsp(&tsp, true), (char **) NULL);
	idx = 0;
	while (s[idx])
	{
		while (s[idx] && ft_strchr(qes.blank, s[idx]))
		{
			++idx;
			if (qes.soloblnk)
				break ;
		}
		if (s[idx] && !set_tsl(&tsp, idx))
			return (cleanup_tsp(&tsp, true), (char **) NULL);
		advance_comment(s, &idx, qes);
		if (preproc_qesstr(s, &idx, &qes, &tsp) == 2)
			continue ;
		advance_qesstr(s, &idx, &qes, &tsp);
		postproc_qesstr(s, &idx, &qes, &tsp);
	}
	return (transform_list(&tsp, s));
}
