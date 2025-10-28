/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 11:40:59 by rhaas             #+#    #+#             */
/*   Updated: 2025/10/28 13:46:18 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t	get_envsize(char **const env)
{
	char	**env2;
	size_t	envsize;

	if (!env)
		return (0);
	env2 = env;
	envsize = 0;
	while (*(env2++))
		++envsize;
	return (envsize);
}

size_t	check_cpy_env(t_cd *pcd, bool env_init)
{
	char				**newenv;
	size_t				envsz;
	size_t				idx;

	envsz = get_envsize(pcd->msh_env);
	if (envsz < pcd->envrsvd)
		return (envsz);
	newenv = malloc((envsz + ENV_XTRA_ELEMS + 1) * sizeof(char *));
	if (!newenv)
		return (0);
	pcd->envrsvd = envsz + ENV_XTRA_ELEMS;
	idx = -1;
	while ((pcd->msh_env)[++idx])
	{
		if (env_init)
			newenv[idx] = ft_strdup((pcd->msh_env)[idx]);
		else
			newenv[idx] = (pcd->msh_env)[idx];
	}
	newenv[idx] = (char *) NULL;
	if (!env_init)
		free(pcd->msh_env);
	pcd->msh_env = newenv;
	environ = pcd->msh_env;
	return (envsz);
}

void	msh_error(char const **errstrs, int nstrs, int errnum)
{
	ft_dprintf(2, "%s: ", "msh");
	while (nstrs--)
	{
		ft_dprintf(2, "%s", *(errstrs++));
		if (nstrs)
			ft_dprintf(2, ": ");
	}
	if (errnum > 0)
	{
		errno = errnum;
		ft_dprintf(2, ": %s\n", strerror(errno));
	}
	else
		ft_dprintf(2, "\n");
}
