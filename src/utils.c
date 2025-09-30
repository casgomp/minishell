/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 11:40:59 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/30 19:20:14 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <stdlib.h>

size_t	envsize(char **const env)
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

char	**check_cpy_env(char **oldenv, size_t *pexternenvsz, bool free_oldenv)
{
	static size_t const	xtraspace = 32;
	char				**newenv;
	char				**envelem;
	size_t				envsz;

	if (!oldenv)
		return ((char **) NULL);
	envsz = envsize(oldenv);
	if (envsz < (*pexternenvsz))
		return (oldenv);
	newenv = malloc((envsz + xtraspace) * sizeof(char *));
	if (!newenv)
		return (*pexternenvsz = 0, (char **) NULL);
	*pexternenvsz += xtraspace;
	envelem = newenv;
	while (*oldenv)
		*(envelem++) = *(oldenv++);
	*envelem = (char *) NULL;
	if (free_oldenv)
		rh_free_simple_array((void **)oldenv);
	return (newenv);
}
