/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_rmquotes_expand.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 17:14:25 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/17 12:21:46 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MS_RMQUOTES_EXPAND_H
# define MS_RMQUOTES_EXPAND_H

#include <stddef.h>

typedef struct s_strstart_len
{
	char					*pcstart;
	size_t					len;
	struct s_strstart_len	*next;
}	t_strstart_len;

typedef struct s_unqut_expd_data
{
	char			*in;
	char			*uestr;
	char			actvq;
	t_strstart_len	*tsl_orig;
	t_strstart_len	*tsl;
	size_t			uestrlen;
}	t_uedata;

typedef struct s_quotes_esc_dict
{
	char const *q;
	char const esc;
	char const expnd;
	char *(*dict)(char const *const);
}	t_qed;

char	*ms_rmquotes_expand(char const *const qutdstr, t_qed tqed);

#endif // MS_RMQUOTES_EXPAND_H
