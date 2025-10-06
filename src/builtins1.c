/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 19:23:31 by rhaas             #+#    #+#             */
/*   Updated: 2025/10/06 18:58:12 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "msh_rmquotes_expand.h"

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static int	msh_echo(char **args)
{
	int		ret;
	bool	newline;

	ret = 0;
	newline = true;
	while (*(++args) && !ft_strcmp(*args, "-n"))
		newline = false;
	--args;
	while (*(++args) && ret >= 0)
	{
		ret = ft_printf("%s", *args);
		if (ret >= 0 && *(args + 1))
			ret = ft_printf("%c", ' ');
	}
	if (newline && ret >= 0)
		ret = ft_printf("%c", '\n');
	return (ret < 0);
}

static int	msh_exit(t_cd *pcd, char **args)
{
	int	success;

	if (args[1])
	{
		pcd->exit_status = ft_atoll_check_input(args[1], INT_MAX, &success);
		if (success < (int)ft_strlen(args[1]))
		{
			ft_printf("Dummy str: bash: exit: 82949672970920399492: "
						"numeric argument required\n");
			return (1);
		}
	}
	if (pcd->interactive)
		ft_printf("exit\n");
	pcd->exit = true;
	return (pcd->exit_status %= 256);
}

static void	msh_perror(char const *const errstr, int errnum)
{
	ft_fdprintf(2, "%s: ", "msh");
	errno = errnum;
	if (errstr)
		perror(errstr);
}

static int	msh_cd(char **args)
{
	char	buf[1024];
	char	*path;

	if (!args)
		return (1);
	if (!args[1])
		path = ft_strdup(getenv("HOME"));
	else if (args[2])
		return (msh_perror(__func__+4, E2BIG), 1);
	else
		path = ft_strdup(args[1]);
	if (path && !(!ft_strcmp(path, "")))
	{
		if (chdir(path) == -1)
		{
			*buf = '\0';
			ft_strlcat(buf, path, 1024);
			ft_strlcat(buf, ": ", 1024);
			ft_strlcat(buf, __func__+4, 1024);
			return (free(path), msh_perror(buf, errno), 1);
		}
	}
	return (0);
}

// returns the shell exit status
static int	msh_pwd(/*t_cd *pcd , char **args */)
{
	char *const cwd = getcwd((char *)NULL, 0);
	if (!cwd)
		return (1);
	ft_printf("%s\n", cwd);
	free(cwd);
	return (0);
}

static int	msh_env(/* t_cd const *const pcd */)
{
	char 	**env2;
		
	env2 = environ;	
	while (*env2)
		ft_printf("%s\n", *(env2++));
	return (0);
}

// export with no options
static int	msh_export(t_cd *pcd, char **args)
{
	char	**env;
	char	*pc;
	size_t	sublen;
	size_t	idx;
	bool	isfirstchar;
	int		ret;
	
//errors (exit val $?==1)
// + not an identifier if var name has not allowed chars
// + export "" gives export: not valid in this context:
	if (!(args && args[0]))
		return (1);
	if (!args[1])
		return (msh_env());
	ret = 0;
	while (*(++args))
	{
		if (!(**args))
		{
			ft_fdprintf(STDERR_FILENO, "%s: %s: '%s': not a valid identifier\n", pcd->mshid, __func__+4, *args);
			ret = 1;
			continue;
		}
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
			// pc = ft_substr(*args, 0, sublen);
			ft_fdprintf(STDERR_FILENO, "%s: %s: '%s': not a valid identifier\n", pcd->mshid, __func__+4, *args);
			// free(pc);
			ret = 1;
			continue ;
		}
		if (!pc)
			continue ;
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
			pcd->envsize = check_cpy_env(&environ, &(pcd->envrsvd), false);
			environ[pcd->envsize++] = ft_strdup(*args);
			environ[pcd->envsize] = (char *) NULL;
		}
	}
	return (ret);
}

// unset with no options
static int	msh_unset(t_cd *pcd, char **args)
{
	char	**env;
	
	while (*(++args))
	{
		env = environ;
		while (*env && ft_strncmp(*args, *env, ft_strlen(*args)))
			++env;
		if (*env)
			--(pcd->envsize);
		free(*(env));
		while (*(++env))
		{
			// free(*(env++));
			*(env - 1) = *(env);
		}
		*(env - 1) = (char *) NULL;
	}
	return (0);
}

int	msh_builtins(t_cd *pcd, char **const args)
{
	if (!ft_strcmp(args[0], "cd"))
		pcd->exit_status = (msh_cd(args));
	else if (!ft_strcmp(args[0], "echo"))
		pcd->exit_status = (msh_echo(args));
	else if (!ft_strcmp(args[0], "env"))
		pcd->exit_status = (msh_env(/* pcd */));
	else if (!ft_strcmp(args[0], "exit"))
		pcd->exit_status = (msh_exit(pcd, args));
	else if (!ft_strcmp(args[0], "export"))
		pcd->exit_status = (msh_export(pcd, args));
	else if (!ft_strcmp(args[0], "pwd"))
		pcd->exit_status = (msh_pwd());
	else if (!ft_strcmp(args[0], "unset"))
		pcd->exit_status = (msh_unset(pcd, args));
	else
		return (false);
	return (true);
}
