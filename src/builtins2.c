/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 19:23:31 by rhaas             #+#    #+#             */
/*   Updated: 2025/10/27 15:16:33 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "msh_builtins.h"
#include "msh_compile_cmds.h"

#include "libft.h"

#include <stdlib.h>
#include <unistd.h>

int	msh_env(void)
{
	char	**env2;

	env2 = environ;
	while (*env2)
		ft_printf("%s\n", *(env2++));
	return (0);
}

static bool	msh_export_check_arg(char const *const arg, size_t *const psublen,
	int *preturn, char const *const funcname)
{
	char	*pc;
	bool	isfirstchar;	
	size_t	idx;
	char	buf[1024];

	*psublen = ft_strlen(arg);
	pc = ft_strchr(arg, '=');
	if (pc)
		*psublen = pc - arg;
	idx = 0;
	isfirstchar = true;
	while (idx < *psublen && msh_isvalidvarchar((arg)[idx], &isfirstchar))
		++idx;
	if (*psublen == 0 || idx < *psublen)
	{
		ft_sprintf(buf, "`%s\'", arg);
		msh_error((char const *[]){funcname, buf,
			"not a valid identifier"}, 3, -1);
		*preturn = 1;
	}
	if ((arg)[*psublen] != '=')
		return (false);
	return (true);
}

static void	msh_export_setenv(t_cd *pcd, char const *const arg, char **env)
{
	size_t	idx;

	if (*env)
	{
		free(*env);
		*env = ft_strdup(arg);
	}
	else
	{
		idx = check_cpy_env(&environ, &(pcd->envrsvd), false);
		environ[idx] = ft_strdup(arg);
		environ[++idx] = (char *) NULL;
	}
}

// export with no options
int	msh_export(t_cd *pcd, char **args)
{
	char	**env;
	size_t	sublen;
	int		ret;

	if (!(args && args[0]))
		return (1);
	if (!args[1])
		return (msh_env());
	ret = 0;
	while (*(++args))
	{
		if (!msh_export_check_arg(*args, &sublen, &ret, __func__ + 4))
			continue ;
		env = environ;
		while (*env && ft_strncmp(*args, *env, sublen + 1))
			++env;
		msh_export_setenv(pcd, *args, env);
	}
	return (ret);
}

// unset with no options
int	msh_unset(char **args)
{
	char	**env;
	size_t	arglen;

	while (*(++args))
	{
		if (ft_strchr(*args, '=') != (char *) NULL)
			continue ;
		arglen = ft_strlen(*args);
		env = environ;
		while (*env && !(ft_strncmp(*args, *env, arglen) == 0
				&& (*env)[arglen] == '='))
		{
			++env;
		}
		if (*env)
		{
			free(*(env));
			while (*(++env))
				*(env - 1) = *(env);
			*(env - 1) = (char *) NULL;
		}
	}
	return (0);
}
