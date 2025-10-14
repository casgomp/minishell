/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rh_split_qed.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 17:14:25 by rhaas             #+#    #+#             */
/*   Updated: 2025/10/14 11:26:22 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RH_SPLIT_QED_H
# define RH_SPLIT_QED_H

# include <stdbool.h>

// q		: string of quote characters, default "\"'"
// esc		: escape char, default '\'
// blank	: string of blanks, separating tokens, default " \t"
// metachr	: string of metacharacters that also separate tokens (as blanks do)
// 			  but are not removed from token list
// 			  because they have additional meaning, default
typedef struct s_quotes_esc_split
{
	char	*q;
	char	esc;
	char	*blank;
	char	*metachr;
}	t_qes;
char	**rh_split_quotes(char const *const s, t_qes const qes);

// q and esc: see above, same as for t_qes
// expnd	: char introducing (parameter) expansion
// dict		: func (ptr) that does the param expansion operation
// 			  takes a param name and returns a value str
// 			  additional data can be passed to that func using dict_data
// validchar: func (ptr) that checks one by one if a char is a valid part of
// 			  a parameter name. Takes a char and returns (bool)valid/not valid
// 			  additional data can be passed to that func using validchar_data
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
char	**rh_rmqed(char const *const qutdstr, t_qed tqed);

typedef struct s_split_quotes_esc_dict
{
	char	*q;
	char	esc;
	char	*blank;
	char	*metachr;
	char	*ifs;

	char	expnd;
	void	*dict_data;
	char	*(*dict)(char const *const key, void *dict_data);
	void	*validchar_data;
	bool	(*validchar)(char const c, void *validchar_data);
}	t_sqed;
char	**rh_split_rmqed(char const *const to_split, t_sqed const sqed);

#endif // RH_SPLIT_QED_H
