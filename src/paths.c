/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 16:41:08 by pecastro          #+#    #+#             */
/*   Updated: 2025/10/21 17:44:56 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "msh_rmquotes_expand.h"
#include "libft.h"

#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

static int	free_splitstrs(char **split)
{
	char **const	spltorig = split;

	while (*split)
		free(*(split++));
	free(spltorig);
	split = (char **) NULL;
	return (1);
}

static int	access_check_path(char *pathstr)
{
	if(access(pathstr, X_OK) != -1)
		return(1);
	return(0);
}

static int	compose_check_path(const char *path, char *cmdexe, char *pathstr, size_t bufferlen)
{
	struct stat	statbuf;

	if (!(cmdexe && pathstr))
		return (0);
	pathstr[0] = '\0';
	if (path && *path)
	{
		ft_strlcat(pathstr, path, bufferlen);
		ft_strlcat(pathstr, "/", bufferlen);
	}
	ft_strlcat(pathstr, cmdexe, bufferlen);
	if(stat(pathstr, &statbuf) != -1)
		return(1);
	return(0);
}

char	*compose_check_cmd(t_cd *pcd, t_cmd *curcmd, char **epaths, size_t epaths_len)
{
	char			**testpath;
	char			*path;
	char			*pcmd;
	size_t			bufferlen;
	int				check;

	pcmd = curcmd->args[0];
	bufferlen = epaths_len + ft_strlen(pcmd) + 1;
	path = ft_calloc(bufferlen, sizeof(char));
	if (!path)
		return (pcd->exit_status = EXIT_FAILURE, NULL);
	check = 0;
	if (ft_strchr(pcmd, '/'))
    	check = compose_check_path(NULL, pcmd, path, bufferlen);
	else
	{
		testpath = epaths;
		while (*testpath && !check)
		{
			check = compose_check_path(*testpath, pcmd, path, bufferlen);
			++testpath;
		}
	}
	if (!check)
		return (perror(pcmd), pcd->exit_status = 127, free_splitstrs(epaths), free(path), NULL);
	check = access_check_path(path);
	if (!check)
		return (perror(pcmd), pcd->exit_status = 126, free_splitstrs(epaths), free(path), NULL);
	return(free_splitstrs(epaths), pcd->exit_status = EXIT_SUCCESS, path);
}

char	**ft_env_paths(size_t *len)
{
	char	**envvar;
	char	*pathvarstr;
	char	**paths;

	envvar = environ;
	while (ft_strncmp(*envvar, "PATH=", 5))
		++ envvar;
	pathvarstr = ft_substr(*envvar, 5, ft_strlen(*envvar));
	if (!pathvarstr)
		return ((char **) NULL);
	paths = ft_split(pathvarstr, ':');
	if (!paths)
		return ((char **) NULL);
	*len = ft_strlen(pathvarstr);
	free(pathvarstr);
	return(paths);
}