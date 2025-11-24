/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 16:41:08 by pecastro          #+#    #+#             */
/*   Updated: 2025/11/21 14:05:44 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "msh_builtins.h"
#include "libft.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

static int	compose_check_path(const char *path, 
	t_cmd *cmd, t_cd *pcd, t_cmd_ctx *ctx)
{
	char		*pathstr;
	struct stat	statbuf;

	pathstr = cmd->path;
	if (!(cmd->args[0] && pathstr))
		return (0);
	pathstr[0] = '\0';
	if (path && *path)
	{
		ft_strlcat(pathstr, path, cmd->bufferlen);
		ft_strlcat(pathstr, "/", cmd->bufferlen);
	}
	ft_strlcat(pathstr, cmd->args[0], cmd->bufferlen);
	if (stat(pathstr, &statbuf) != -1)
	{
		if (access(pathstr, X_OK) == -1)
			return (errno);
		if (access(pathstr, R_OK) == -1)
			return (errno);
		if (!ctx->parpth)
			pcd->exit_status = EXIT_SUCCESS;
		return (1);
	}
	return (errno);
}

int	compose_check_cmd_get_path(t_cd *pcd, t_cmd_ctx *ctx, t_cmd *cmd)
{
	int		check;
	bool	flag;
	char	**testpath;

	check = 0;
	flag = 0;
	fail_stat_exit(pcd, ctx);
	if (ft_strchr(cmd->args[0], '/'))
	{
		check = compose_check_path(NULL, cmd, pcd, ctx);
		if (check == 13)
			cmd_no_permissions(pcd, cmd, &flag, ctx);
	}
	else
	{
		testpath = ctx->epaths;
		while (*testpath && check != 1)
		{
			check = compose_check_path(*testpath, cmd, pcd, ctx);
			if (check == 13 && flag == 0)
				cmd_no_permissions(pcd, cmd, &flag, ctx);
			++testpath;
		}
	}
	return (check);
}

int	compose_check_cmd(t_cd *pcd, t_cmd *cmd, t_cmd_ctx *ctx)
{
	char			*pcmd;

	pcmd = cmd->args[0];
	cmd->bufferlen = ctx->epaths_len + ft_strlen(pcmd) + 1;
	if (cmd->path)
		free(cmd->path);
	cmd->path = ft_calloc(cmd->bufferlen, sizeof(char));
	if (!cmd->path)
		return (pcd->exit_status = 1, 0);
	if (compose_check_cmd_get_path(pcd, ctx, cmd) != 1)
		return (0);
	return (1);
}

char	**ft_env_paths(t_cd *pcd, size_t *len)
{
	char	**envvar;
	char	*pathvarstr;
	char	**paths;

	envvar = pcd->msh_env;
	while (*envvar)
	{
		if (!ft_strncmp(*envvar, "PATH=", 5))
			break ;
		++ envvar;
	}
	if (*envvar == NULL)
		pathvarstr = ft_strdup("");
	else
		pathvarstr = ft_substr(*envvar, 5, ft_strlen(*envvar));
	if (!pathvarstr)
		return ((char **) NULL);
	paths = ft_split(pathvarstr, ':');
	if (!paths)
		return ((char **) NULL);
	*len = ft_strlen(pathvarstr);
	free(pathvarstr);
	return (paths);
}
