/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 11:40:59 by rhaas             #+#    #+#             */
/*   Updated: 2025/10/14 08:27:58 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void	msh_error(char const **errstrs, int nstrs, int errnum)
{
	ft_fdprintf(2, "%s: ", "msh");
	while (nstrs--)
	{
		ft_fdprintf(2, "%s", *(errstrs++));
		if (nstrs)
			ft_fdprintf(2, ": ");
	}
	if (errnum > 0)
	{
		errno = errnum;
		ft_fdprintf(2, ": %s\n", strerror(errno));	
	}
	else
		ft_fdprintf(2, "\n");
}
