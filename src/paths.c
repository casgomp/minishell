/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 16:41:08 by pecastro          #+#    #+#             */
/*   Updated: 2025/11/15 16:19:59 by rhaas            ###   ########.fr       */
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

int	free_splitstrs(char **split)
{
	char **const	spltorig = split;

	while (*split)
		free(*(split++));
	free(spltorig);
	split = (char **) NULL;
	return (1);
}

static int	compose_check_path(const char *path, char *cmdexe, char *pathstr,
			size_t bufferlen, t_cd *pcd)
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
	if (stat(pathstr, &statbuf) != -1)
	{
		if (access(pathstr, X_OK) == -1)
			return (errno);
		if (access(pathstr, R_OK) == -1)
			return (errno);
		return (pcd->exit_status = EXIT_SUCCESS, 1);
	}
	return (errno);
}

int	compose_check_cmd_get_path(t_cd *pcd, t_cmd_ctx *ctx, t_cmd *cmd, char **path_noaccess)
{
	int				check;
	int				flag;
	char			**testpath;

	check = 0;
	flag = 0;
	if (ft_strchr(cmd->args[0], '/'))
		check = compose_check_path(NULL, cmd->args[0], cmd->path, cmd->bufferlen, pcd);
	else
	{
		pcd->exit_status = 127;
		testpath = ctx->epaths;
		while (*testpath && check != 1)
		{
			check = compose_check_path(*testpath, cmd->args[0], cmd->path, cmd->bufferlen, pcd);
			if (check == 13 && flag == 0)
			{
				flag = 1;
				*path_noaccess = ft_strdup(cmd->path);
				pcd->exit_status = 126;
			}
			++testpath;
		}
	}
	return (check);
}

//char	*compose_check_cmd(t_cd *pcd, t_cmd *curcmd, char **epaths,
//			size_t epaths_len)
int	compose_check_cmd(t_cd *pcd, t_cmd *cmd, t_cmd_ctx *ctx, char **path_noaccess)
{
	//char			**testpath;
	//char			*path;
	char			*pcmd;
	//size_t			bufferlen;
	//int				check;
	//int				flag;

	pcmd = cmd->args[0];
	cmd->bufferlen = ctx->epaths_len + ft_strlen(pcmd) + 1;
	//path = ft_calloc(bufferlen, sizeof(char));
	//if (!path)
		//return (pcd->exit_status = 1, 0);
	cmd->path = ft_calloc(cmd->bufferlen, sizeof(char));
	if (!cmd->path)
		return (pcd->exit_status = 1, 0);
	/*check = 0;
	flag = 0;
	if (ft_strchr(pcmd, '/'))
		check = compose_check_path(NULL, pcmd, path, bufferlen, pcd);
	else
	{
		pcd->exit_status = 127;
		testpath = ctx->epaths;
		while (*testpath && check != 1)
		{
			check = compose_check_path(*testpath, pcmd, path, bufferlen, pcd);
			//ft_printf("compose_check_cmd: check/errno = %d\n", check);
			if (check == 13 && flag == 0)
			{
				flag = 1;
				*path_noaccess = ft_strdup(path);
				pcd->exit_status = 126;
			}
			++testpath;
		}
	}*/
	//if (check != 1)
		//return (free(path), 0);
	//if (check != 1)
	if (compose_check_cmd_get_path(pcd, ctx, cmd, path_noaccess) != 1)
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
	return(paths);
}