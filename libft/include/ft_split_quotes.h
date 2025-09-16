/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_quotes.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 17:14:25 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/16 13:59:00 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SPLIT_QUOTES_H
# define FT_SPLIT_QUOTES_H

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

typedef struct s_quotes_esc_split
{
	char splt;
	char *multisplt;
	char *q;
	char esc;
}	t_qes;

#endif
