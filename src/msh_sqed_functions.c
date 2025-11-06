/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_sqed_functions.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:12:30 by rhaas             #+#    #+#             */
/*   Updated: 2025/11/06 19:51:32 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

#include <stdlib.h>

char	*msh_dict(char const *const key, void *dict_data)
{
	t_cd *const	pcd = (t_cd *)dict_data;
	char		*exitstatus;
	static char	exitstatbuff[8];

	if (!ft_strcmp(key, "?"))
	{
		exitstatus = ft_itoa(pcd->exit_status);
		ft_strlcpy(exitstatbuff, exitstatus, 8);
		free(exitstatus);
		return (exitstatbuff);
	}
	else
		return (getenv(key));
}

bool	msh_isvalidvarchar(char const c, void *validchar_data)
{
	static char					firstc;
	static char const *const	special = "?";
	bool *const					isfirstc = (bool *)validchar_data;

	if (!c)
		return (false);
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
