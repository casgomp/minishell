/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_rmquotes_expand.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:13:03 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/23 16:44:54 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MS_RMQUOTES_EXPAND_H
# define MS_RMQUOTES_EXPAND_H

# include <stddef.h>

typedef struct s_strstart_len
{
	char					*pcstart;
	size_t					len;
	struct s_strstart_len	*next;
}	t_strstart_len;

typedef struct s_unqut_expd_data
{
	char			*instr;
	char			actvq;
	t_strstart_len	*tsl_orig;
	t_strstart_len	*tsl;
	char			*uestr;
	size_t			uestrlen;
}	t_uedata;

typedef struct s_quotes_esc_dict
{
	char	*q;
	char	esc;
	char	expnd;
	void	*data;
	char	*(*dict)(char const *const, void *data);
}	t_qed;

char	*ms_rmqts_esc_xpnd(char const *const qutdstr, t_qed tqed);

#endif // MS_RMQUOTES_EXPAND_H
