/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_rmquotes_expand.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:12:30 by rhaas             #+#    #+#             */
/*   Updated: 2025/10/06 14:17:11 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <stdlib.h>

char	*msh_dict(char const *const key, void *dict_data)
{
	t_cd *const	pcd = (t_cd *)dict_data;

	if (!ft_strcmp(key, "?"))
		return (ft_itoa(pcd->exit_status));
	else
		return (getenv(key));
}

// minishell_isAllowedBashVariableCharacter
// https://thelinuxcode.com/bash-variable-name-rules-legal-illegal/
bool	msh_isvalidvarchar(char const c, void *validchar_data)
{
	static char					firstc;
	static char const *const	special = "?";
	bool *const					isfirstc = (bool *)validchar_data;

	if (*isfirstc)
	{
		firstc = c;
		*isfirstc = false;
		if (ft_isdigit(c))
			return (false);
		if (ft_strchr(special, c))
			return (true);
	}
	else if (ft_strchr(special, firstc))
		return (false);
	return (ft_isalnum(c) || c == '_');
}
