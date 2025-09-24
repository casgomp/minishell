/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:37:27 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/24 12:55:57 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "libft.h"

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

extern char	**environ;

static int	free_splitstrs(char **split)
{
	char **const	spltorig = split;

	while (*split)
		free(*(split++));
	free(spltorig);
	split = (char **) NULL;
	return (1);
}

static char	**get_paths(int *pbuflen)
{
	// char	**envvar;
	char	*pathvarstr;
	char	**paths;

	// envvar = environ;
	// while (ft_strncmp(*envvar, "PATH=", 5))
	// 	++envvar;
	while (ft_strncmp(*environ, "PATH=", 5))
		++environ;
	// pathvarstr = ft_substr(*envvar, 5, ft_strlen(*envvar));
	pathvarstr = ft_substr(*environ, 5, ft_strlen(*environ));
	if (!pathvarstr)
		return ((char **) NULL);
	paths = ft_split(pathvarstr, ':');
	if (!paths)
		return ((char **) NULL);
	*pbuflen += 1 + ft_strlen(pathvarstr);
	free(pathvarstr);
	return (paths);
}

static int	compose_check_path(char const *const path, char const *const cmd,
								char *const pathstr, size_t bufferlen)
{
	if (!(cmd && pathstr))
		return (0);
	pathstr[0] = '\0';
	if (path && *path)
	{
		ft_strlcat(pathstr, path, bufferlen);
		ft_strlcat(pathstr, "/", bufferlen);
	}
	ft_strlcat(pathstr, cmd, bufferlen);
	if (access(pathstr, X_OK) != -1)
		return (1);
	return (0);
}

static char	*compose_check_cmd(char *const pcmd)
{
	char	**paths;
	char	**testpath;
	char	*path;
	int		bufferlen;

	bufferlen = ft_strlen(pcmd) + 1;
	paths = get_paths(&bufferlen);
	path = ft_calloc(bufferlen, sizeof(char));
	if (!path)
		return (free_splitstrs(paths), (char *) NULL);
	if (ft_strchr(pcmd, '/'))
	{
		if (compose_check_path(NULL, pcmd, path, bufferlen))
			return (free_splitstrs(paths), path);
		else
			return (perror(pcmd), free_splitstrs(paths), free(path), NULL);
	}
	testpath = paths;
	while (*testpath)
	{
		if (compose_check_path(*testpath, pcmd, path, bufferlen))
			return (free_splitstrs(paths), path);
		++testpath;
	}
	return (perror(pcmd), free_splitstrs(paths), free(path), (char *) NULL);
}

int	run_cmd(char *args, t_fds *const pfds)
{
	char	**cmdargs;
	char	*cmd;
	pid_t	pid;

	cmdargs = ft_split_quotes(args, ' ', "\"\'", true);
	if (!(*cmdargs))
		return (0);
	cmd = compose_check_cmd(cmdargs[0]);
	if (!cmd)
		return (free_splitstrs(cmdargs), 0);
	free (cmdargs[0]);
	cmdargs[0] = cmd;
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		dup2 (pfds->pipefd[1], STDIN_FILENO);
		dup2 (pfds->file1, STDOUT_FILENO);
		close(pfds->file2);
		close(pfds->file1);
		close(pfds->pipefd[1]);
		close(pfds->pipefd[0]);

		execve(cmdargs[0], cmdargs, environ);
		perror("execve");
		exit(EXIT_FAILURE);
	}
	return (free_splitstrs(cmdargs), 1);
}
