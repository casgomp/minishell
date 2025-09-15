/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_rmquotes_expand.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 17:14:25 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/15 19:52:41 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SPLIT_QUOTES_H
# define FT_SPLIT_QUOTES_H

#include <stddef.h>

typedef struct s_strstart_len
{
	char					*pcstart;
	size_t					len;
	struct s_strstart_len	*next;
}	t_strstart_len;

typedef struct s_unqut_expd
{
	char			*in;
	char			*uestr;
	char			actvq;
	t_strstart_len	*tsl_orig;
	t_strstart_len	*tsl;
	size_t			uestrlen;
}	t_unqut_expd;

char	*ms_rmquotes_expand(char const *const qutdstr,
			char const *const q, char const esc);

#endif
