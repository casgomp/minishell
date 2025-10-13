/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 16:41:08 by pecastro          #+#    #+#             */
/*   Updated: 2025/10/13 18:44:42 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "msh_rmquotes_expand.h"
#include "libft.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <unistd.h>

char	**ft_env_paths(void)
{
	int		i;

	i = 0;
	while (environ[i])
	{
		if (ft_strncmp(environ[i], "PATH=", 5) == 0)
			return (ft_split(environ[i] + 5, ':'));
		i ++;
	}
	return (NULL);
}

//function to check stat() 
char	*cmd_valid_s(some_struct.epaths????????)
{
	//each call to stat must be preceded by appending cmd (i.e. args[0]) to path.

	//when correct env path is found, then path is stored in variable to use in access.
	
	//if invalid cmd, calling process must exit(127) after opening files, etc.
}


//function to check access()
	//if correct then it can be used for execve.
	//will only fail if command executable file has no permissions and exit(126).
