/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 19:23:31 by rhaas             #+#    #+#             */
/*   Updated: 2025/10/16 18:41:54 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "msh_builtins.h"
#include "msh_rmquotes_expand.h"

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

// export with no options
int	msh_export(t_cd *pcd, char **args)
{
	char	**env;
	char	*pc;
	size_t	sublen;
	size_t	idx;
	bool	isfirstchar;
	int		ret;

	if (!(args && args[0]))
		return (1);
	if (!args[1])
		return (msh_env());
	ret = 0;
	while (*(++args))
	{
		sublen = ft_strlen(*args);
		pc = ft_strchr(*args, '=');
		if (pc)
			sublen = pc - *args;
		idx = 0;
		isfirstchar = true;
		while (idx < sublen && msh_isvalidvarchar((*args)[idx], &isfirstchar))
				++idx;
		if (sublen == 0 || (*args)[idx] != '=')
		{
			// ft_dprintf(STDERR_FILENO, "%s: %s: `%s': not a valid identifier\n",
			// 	pcd->mshid, __func__ + 4, *args);
			char buf[1024];
			ft_sprintf(buf, "`%s\'", *args);
			msh_error((char const *[]){__func__ + 4, buf,
				"not a valid identifier"}, 3, -1);
			ret = 1;
			continue ;
		}
		env = environ;
		while (*env && ft_strncmp(*args, *env, sublen + 1))
			++env;
		if (*env)
		{
			free(*env);
			*env = ft_strdup(*args);
		}
		else
		{
			idx = check_cpy_env(&environ, &(pcd->envrsvd), false);
			environ[idx] = ft_strdup(*args);
			environ[++idx] = (char *) NULL;
		}
	}
	return (ret);
}

// unset with no options
int	msh_unset(char **args)
{
	char	**env;

	while (*(++args))
	{
		env = environ;
		while (*env && ft_strncmp(*args, *env, ft_strlen(*args)))
			++env;
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
