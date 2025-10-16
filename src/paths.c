/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 16:41:08 by pecastro          #+#    #+#             */
/*   Updated: 2025/10/16 21:08:07 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "msh_rmquotes_expand.h"
#include "libft.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <unistd.h>

char	**ft_env_paths(int *len)
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

	/*
	int		i;

	i = 0;
	while (environ[i])
	{
		if (ft_strncmp(environ[i], "PATH=", 5) == 0)
			return (ft_split(environ[i] + 5, ':'));
		i ++;
	}
	return (NULL);*/
}

static int	compose_check_cmd(t_cd *pcd, t_cmd *curcmd, char **epaths, int epaths_len)
{
	char			**testpath;
	char			*path;
	int				bufferlen;

	bufferlen = 0;
	bufferlen = epaths_len + ft_strlen(curcmd->args[0]) + 1;
	path = ft_calloc(bufferlen, sizeof(char));
	if (!path)
	{
		//free epaths?
		//return NULL? and set cd->exit to what?
	}
	if (ft_strchr(curcmd->args[0], '/'))
	{
		if (compose_check_path(NULL, curcmd->args[0], path, bufferlen))
		{
			//function to free epaths
			return (path);
		}
		else
		{
			//perror(curcmd->args[0])
			//function to free epaths //can i free it here? or in calling function?
			free(path);
			pcd->exit_status = 127;
			//return (NULL)?; //This is wrong because child still opens files and sets redirections before exiting
		}
	}
	testpath = epaths;
	while (*testpath)
	{
		if (compose_check_path(*testpath, curcmd->args[0], path, bufferlen))
		{
			//function to free epaths
			return (path);
		}
		++testpath;
	}
	//perror(curcmd->args[0])
	//function to free epaths //can i free it here? or in calling function?
	free(path);
	pcd->exit_status = 127;
	//return (NULL)?;//This is wrong because child still opens files and sets redirections before exiting
	



}

char	*compose_check_path(t_cmd *cmd, char **epaths)
{
	struct stat	*statbuf;
	int			bufferlen;
	int			i;

	i = 0;
	while (epaths[i])
	{
		//each call to stat must be preceded by appending cmd (i.e. args[0]) to path.
		if (ft_strchr(cmd->args[0], '/'))
		{
			
		}
		
		//when correct env path is found, then path is stored in variable to use in access.
		stat()
		//if invalid cmd, calling process must exit(127) after opening files, etc.
		i ++;
	}
}


//function to check access()
int	access_check_path()
	//if correct then it can be used for execve.
	//will only fail if command executable file has no permissions and exit(126).
