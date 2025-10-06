/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_rmquotes_expand.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:13:03 by rhaas             #+#    #+#             */
/*   Updated: 2025/10/06 14:18:11 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSH_RMQUOTES_EXPAND_H
# define MSH_RMQUOTES_EXPAND_H

# include <stdbool.h>

char	*msh_dict(char const *const key, void *dict_data);
bool	msh_isvalidvarchar(char const c, void *validchar_data);

#endif // MSH_RMQUOTES_EXPAND_H
