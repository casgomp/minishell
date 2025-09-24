/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rh_split_qed.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 17:14:25 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/24 17:06:28 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RH_SPLIT_QED_H
# define RH_SPLIT_QED_H

# include <stdbool.h>

typedef struct s_quotes_esc_split
{
	char	*q;
	char	esc;
	char	*splt;
	char	*specialsplt;
}	t_qes;
char	**rh_split_quotes(char const *const s, t_qes const qes);

typedef struct s_quotes_esc_dict
{
	char	*q;
	char	esc;
	char	expnd;
	void	*dict_data;
	char	*(*dict)(char const *const key, void *dict_data);
	void	*validchar_data;
	bool	(*validchar)(char const c, void *validchar_data);
}	t_qed;
char	*rh_rmqed(char const *const qutdstr, t_qed tqed);

typedef struct s_split_quotes_esc_dict
{
	char	*q;
	char	esc;
	char	*splt;
	char	*specialsplt;

	char	expnd;
	void	*dict_data;
	char	*(*dict)(char const *const key, void *dict_data);
	void	*validchar_data;
	bool	(*validchar)(char const c, void *validchar_data);
}	t_sqed;
char	**rh_split_rmqed(char const *const to_split, t_sqed const sqed);

#endif // RH_SPLIT_QED_H
