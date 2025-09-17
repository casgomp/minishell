/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rh_split_quotes_intern.h                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raphha <raphha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 17:14:25 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/21 13:05:21 by raphha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RH_SPLIT_QUOTES_INTERN_H
# define RH_SPLIT_QUOTES_INTERN_H

# include <stddef.h>

typedef struct s_startlen
{
	int					start;
	size_t				len;
	struct s_startlen	*next;
}	t_startlen;

typedef struct s_splitparams
{
	char		**split;
	t_startlen	*tsl_orig;
	t_startlen	*tsl;
	size_t		n_slelem;
	char		activeq;
}	t_splprms;

int		set_tsl(t_splprms *const ptsp, int idx);
char	**transform_list(t_splprms *const ptsp, char const *const s);
void	cleanup(t_splprms *const ptsp);

#endif // RH_SPLIT_QUOTES_INTERN_H
