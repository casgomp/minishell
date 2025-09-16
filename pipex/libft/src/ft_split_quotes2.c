/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_quotes2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 17:57:26 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/04 21:22:59 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_split_quotes.h"

#include <stdlib.h>

// static void	cleanup(t_splprms *const ptsp)
// {
// 	size_t		idx;
// 	t_startlen	*tmp;

// 	idx = 0;
// 	while (ptsp->split && ptsp->split[idx])
// 		free(ptsp->split[idx++]);
// 	free(ptsp->split);
// 	ptsp->split = (char **) NULL;
// 	while (ptsp->tsl_orig)
// 	{
// 		tmp = ptsp->tsl_orig->next;
// 		free(ptsp->tsl_orig);
// 		ptsp->tsl_orig = tmp;
// 	}
// 	ptsp->tsl_orig = (t_startlen *) NULL;
// }

// static int	set_tsl(t_splprms *const ptsp, int idx)
// {
// 	if (!(ptsp->tsl_orig) || (ptsp->tsl)->len)
// 	{
// 		if (!(ptsp->tsl_orig))
// 		{
// 			ptsp->tsl_orig = malloc(sizeof(t_startlen));
// 			ptsp->tsl = ptsp->tsl_orig;
// 		}
// 		else
// 		{
// 			ptsp->tsl->next = malloc(sizeof(t_startlen));
// 			ptsp->tsl = ptsp->tsl->next;
// 		}
// 		*(ptsp->tsl) = (t_startlen){idx, 0, (t_startlen *) NULL};
// 		++(ptsp->n_slelem);
// 	}
// 	return (ptsp->tsl != (t_startlen *) NULL);
// }

// static int	transform_prep(t_splprms *const ptsp)
// {
// 	if (ptsp->activeq)
// 		return (0);
// 	ptsp->split = (char **)malloc((ptsp->n_slelem + 1) * sizeof(char *));
// 	if (!ptsp->split)
// 		return (0);
// 	ptsp->split[ptsp->n_slelem] = (char *) NULL;
// 	ptsp->tsl = ptsp->tsl_orig;
// 	return (1);
// }

void	remove_quotes(char *const qutdstr, t_splprms *const ptsp,
							char const *const q)
{
	int		idx;
	char	*pc;

	idx = 0;
	while (qutdstr[idx])
	{
		pc = &(qutdstr[idx]);
		if (*pc && ((ptsp->activeq && *pc == ptsp->activeq)
				|| (!ptsp->activeq && ft_strchr(q, *pc))))
		{
			if (!ptsp->activeq)
				ptsp->activeq = *pc;
			else if (ptsp->activeq)
				ptsp->activeq = '\0';
			ft_strlcpy(pc, pc + 1, ft_strlen(pc));
		}
		else if (*pc && ft_strchr("\\", *pc))
		{
			ft_strlcpy(pc, pc + 1, ft_strlen(pc));
			++idx;
		}
		else
			++idx;
	}
}

// static char	**transform_list(t_splprms *const ptsp, char const *const s,
// 							char const *const q, bool const rmq)
// {
// 	int		idx;
// 	char	**splt;

// 	if (!transform_prep(ptsp))
// 		return (cleanup(ptsp), (char **) NULL);
// 	splt = ptsp->split;
// 	idx = 0;
// 	while (ptsp->tsl)
// 	{
// 		splt[idx] = ft_substr(s, ptsp->tsl->start, ptsp->tsl->len);
// 		if (!splt[idx])
// 			return (cleanup(ptsp), (char **) NULL);
// 		if (rmq)
// 			remove_quotes(splt[idx], ptsp, q);
// 		++idx;
// 		ptsp->tsl = ptsp->tsl->next;
// 	}
// 	ptsp->split = (char **) NULL;
// 	return (cleanup(ptsp), splt);
// }

// // Same as ft_split, but takes a string containing quote chars
// // (typically "\\"\\'") that will cancel the effect of the split char
// // in the section between the quote chars
// char	**ft_split_quotes(char const *const s, char const c,
// 						char const *const q, bool const rmq)
// {
// 	t_splprms	tsp;
// 	int			idx;

// 	tsp = (t_splprms){(char **) NULL, (t_startlen *) NULL, NULL, 0, '\0'};
// 	if (!(s && c))
// 		return (cleanup(&tsp), (char **) NULL);
// 	idx = 0;
// 	while (s[idx])
// 	{
// 		while (s[idx] && c == s[idx])
// 			++idx;
// 		if (s[idx] && !set_tsl(&tsp, idx))
// 			return (cleanup(&tsp), (char **) NULL);
// 		if (s[idx] && !tsp.activeq && ft_strchr(q, s[idx]))
// 			tsp.activeq = s[idx++];
// 		while (s[idx]
// 			&& ((!tsp.activeq && !ft_strchr(q, s[idx]) && (s[idx] != c))
// 				|| (tsp.activeq && s[idx] != tsp.activeq)))
// 			idx += 1 + (s[idx] == '\\');
// 		if (tsp.activeq && s[idx] == tsp.activeq && ++idx)
// 			tsp.activeq = '\0';
// 		if (!s[idx] || (!tsp.activeq && s[idx] == c))
// 			tsp.tsl->len = idx - tsp.tsl->start;
// 	}
// 	return (transform_list(&tsp, s, q, rmq));
// }
