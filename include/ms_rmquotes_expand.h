/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_rmquotes_expand.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:13:03 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/24 17:24:03 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MS_RMQUOTES_EXPAND_H
# define MS_RMQUOTES_EXPAND_H

# include <stdbool.h>

char	*ms_dict(char const *const key, void *dict_data);
bool	ms_isvalidvarchar(char const c, void *validchar_data);

#endif // MS_RMQUOTES_EXPAND_H
