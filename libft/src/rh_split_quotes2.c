/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rh_split_quotes2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 17:57:26 by rhaas             #+#    #+#             */
/*   Updated: 2025/10/20 15:07:10 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "rh_split_qed_intern.h"

#include <stdlib.h>

void	cleanup_splt(t_splprms *const ptsp)
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
		return (cleanup_splt(ptsp), (char **) NULL);
	splt = ptsp->split;
	idx = 0;
	while (ptsp->tsl)
	{
		splt[idx] = ft_substr(s, ptsp->tsl->start, ptsp->tsl->len);
		if (!splt[idx])
			return (cleanup_splt(ptsp), (char **) NULL);
		++idx;
		ptsp->tsl = ptsp->tsl->next;
	}
	ptsp->split = (char **) NULL;
	return (cleanup_splt(ptsp), splt);
}
