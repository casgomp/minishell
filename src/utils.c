/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 11:40:59 by rhaas             #+#    #+#             */
/*   Updated: 2025/10/06 19:01:22 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <stdlib.h>

size_t	get_envsize(char **const env)
{
	char 	**env2;
	size_t	envsize;

	if (!env)
		return (0);
	env2 = env;
	envsize = 0;
	while (*(env2++))
		++envsize;
	return (envsize);
}

size_t	check_cpy_env(
	char ***const oldenv,
	size_t * const penvreserved,
	bool env_init)
{
	static size_t const	xtraspace = 1;
	char				**newenv;
	size_t				envsz;
	size_t				idx;

	if (!oldenv)
		return (0);
	envsz = get_envsize(*oldenv);
	if (envsz < (*penvreserved))
		return (envsz);
	newenv = malloc((envsz + xtraspace + 1) * sizeof(char *));
	if (!newenv)
		return (0);
	*penvreserved = envsz + xtraspace;
	idx = -1;
	while ((*oldenv)[++idx])
		if (env_init)
			newenv[idx] = ft_strdup((*oldenv)[idx]);
		else
			newenv[idx] = (*oldenv)[idx];
	newenv[idx] = (char *) NULL;
	*oldenv = newenv;
	return (envsz);
}
