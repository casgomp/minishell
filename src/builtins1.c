/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 19:23:31 by rhaas             #+#    #+#             */
/*   Updated: 2025/11/06 19:06:01 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "msh_builtins.h"
#include "msh_compile_cmds.h"

#include <errno.h>
#include <limits.h>
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
			msh_error((char const *[]){__func__ + 4, args[1],
				"numeric argument required"}, 3, -1);
			pcd->exit_status = 2;
		}
	}
	if (pcd->interactive)
		ft_dprintf(2, "exit\n");
	pcd->exit = true;
	return (pcd->exit_status %= 256);
}

static int	msh_cd(char **args)
{
	char	*path;

	if (!args)
		return (1);
	if (!args[1])
		path = ft_strdup(getenv("HOME"));
	else if (args[2])
		return (msh_error((char const *[]){__func__ + 4,}, 1, E2BIG), 1);
	else
		path = ft_strdup(args[1]);
	if (path && !(!ft_strcmp(path, "")))
	{
		if (chdir(path) == -1)
		{
			msh_error((char const *[]){path, __func__ + 4}, 2, errno);
			return (free(path), 1);
		}
	}
	return (free(path), 0);
}

static int	msh_pwd(void)
{
	char *const	cwd = getcwd((char *) NULL, 0);

	if (!cwd)
		return (1);
	ft_printf("%s\n", cwd);
	free(cwd);
	return (0);
}

int	msh_builtins(t_cd *pcd, char **const args)
{
	if (!ft_strcmp(args[0], "cd"))
		pcd->exit_status = (msh_cd(args));
	else if (!ft_strcmp(args[0], "echo"))
		pcd->exit_status = (msh_echo(args));
	else if (!ft_strcmp(args[0], "env"))
		pcd->exit_status = (msh_env(pcd, (char *) NULL));
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
