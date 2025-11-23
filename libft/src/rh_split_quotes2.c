/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rh_split_quotes2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 17:57:26 by rhaas             #+#    #+#             */
/*   Updated: 2025/11/23 13:24:26 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "rh_split_qed_intern.h"

#include <stdlib.h>

void	postproc_metachars(char const *const s, int *const pidx,
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

void	cleanup_tsp(t_splprms *const ptsp, bool incl_splt)
{
	// size_t		idx;
	t_startlen	*tmp;

	// idx = 0;
	if (incl_splt)
		rh_free_simple_array((void ***)&ptsp->split);
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
	if (!(ptsp->tsl_orig) || (ptsp->tsl)->complete)
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
		*(ptsp->tsl) = (t_startlen){.start = idx, .len = 0,
			.complete = false, .next = (t_startlen *) NULL};
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

	if (!transform_prep(ptsp))
		return (cleanup_tsp(ptsp, true), (char **) NULL);
	idx = 0;
	while (ptsp->tsl)
	{
		ptsp->split[idx] = ft_substr(s, ptsp->tsl->start, ptsp->tsl->len);
		if (!ptsp->split[idx])
			return (cleanup_tsp(ptsp, true), (char **) NULL);
		++idx;
		ptsp->tsl = ptsp->tsl->next;
	}
	return (cleanup_tsp(ptsp, false), ptsp->split);
}
