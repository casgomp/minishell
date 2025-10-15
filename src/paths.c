/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 16:41:08 by pecastro          #+#    #+#             */
/*   Updated: 2025/10/15 20:14:52 by rhaas            ###   ########.fr       */
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

//function to check stat() 
char	*cmd_verif_s(t_cmd *cmd, char **epaths)
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
int	cmd_verif_a()
	//if correct then it can be used for execve.
	//will only fail if command executable file has no permissions and exit(126).
