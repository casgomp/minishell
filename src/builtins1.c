/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 19:23:31 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/30 16:17:36 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static int	msbi_echo(t_cd *pcd, char **args)
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
	pcd->exit_status = 0;
	return (ret >= 0);
}

static int	msbi_exit(t_cd *pcd, char **args)
{
	int	success;

	if (args[1])
	{
		pcd->exit_status = ft_atoll_check_input(args[1], INT_MAX, &success);
		if (success < (int)ft_strlen(args[1]))
			ft_printf("Dummy str: bash: exit: 82949672970920399492: "
						"numeric argument required");
		else
			pcd->exit_status %= 256;
	}
	if (pcd->interactive)
		ft_printf("exit\n");
	pcd->exit = true;
	return (1);
}
static void	msbi_perror(char const *const errstr, int errnum)
{
	ft_fdprintf(2, "%s: ", getenv("SHELL"));
	errno = errnum;
	if (errstr)
		perror(errstr);
}

static int	msbi_cd(char **args)
{
	char	*cwd;
	char	*path;
	// int		ret;

	if (!args)
		return (1);
	// ret = 1;		
	if (!args[1])
		path = ft_strdup(getenv("HOME"));
	else if (args[2])
		return (msbi_perror(__func__, E2BIG), 1);
	else if (args[1][0] == '/')
		path = ft_strdup(args[1]);
	else
	{
		cwd = getcwd((char *)NULL, 0);
		path = ft_strjoin(cwd, "/");
		free (cwd);
		cwd = path;
		path = ft_strjoin(cwd, args[1]);
		free(cwd);
	}
	if (path && ft_strcmp(path, ""))
	{
		if (chdir(path) == -1)
		{
			path = ft_strjoin(args[1], ": ");
			return (msbi_perror(__func__, errno), 1);
		}
	}
	return (free(path), 0);
}

// returns the shell exit status
static int	msbi_pwd(/*t_cd *pcd , char **args */)
{
	char *const cwd = getcwd((char *)NULL, 0);
	if (!cwd)
		return (1);
	ft_printf("%s\n", cwd);
	free(cwd);
	return (0);
}

int	ms_builtins(t_cd *pcd, char **const args)
{
	if (!ft_strcmp(args[0], "cd"))
		pcd->exit_status = (msbi_cd(/* pcd, */ args));
	else if (!ft_strcmp(args[0], "echo"))
		pcd->exit_status = (msbi_echo(pcd, args));
	else if (!ft_strcmp(args[0], "exit"))
		pcd->exit_status = (msbi_exit(pcd, args));
	else if (!ft_strcmp(args[0], "pwd"))
		pcd->exit_status = (msbi_pwd(/*pcd , args */));
	else
		return (false);
	return (true);
}
