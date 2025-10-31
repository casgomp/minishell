/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 13:50:29 by pecastro          #+#    #+#             */
/*   Updated: 2025/10/31 14:10:36 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "msh_builtins.h"
#include "libft.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int	get_heredoc(int fd, char *limiter)
{
	char	*ret;
	char	*prompt;
	int		len;

	len = ft_strlen(limiter);
	prompt = "> ";
	while (1)
	{
		if (write(1, prompt, 2) == -1)
			return (0);
		ret = get_next_line(0);
		if (!ret)
			return (0);
		if (ft_strncmp(ret, limiter, len) == 0 && ret[len] == '\n')
			return (free(ret), 1);
		if (write(fd, ret, ft_strlen(ret)) == -1)
			return (free(ret), 0);
		free (ret);
	}
}

int	redirections_setup(t_io *curredir)
{
	char	*fnmdlm;
	//int		hdfdpipe[2];

	fnmdlm = curredir->fname_delim;
	if (curredir->tredirect == fd2file_trunc || 
		curredir->tredirect == fd2file_append ||
		curredir->tredirect == file2fd)
	{
		/*curredir->old_fd = open(fnmdlm, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		if (curredir->old_fd == -1)
			return (perror(fnmdlm), 0);*/
		dup2(curredir->old_fd, curredir->new_fd);//also check dup2 return and perror?
		close(curredir->old_fd);
	}
	/*needs revision
	else if (curredir->tredirect == heredoc2fd)
	{
		dup2(hdfdpipe[0], curredir->new_fd);
		close(curredir->old_fd);
	}*/
	else if (curredir->tredirect == heredoc2fd)
	{
		dup2(curredir->old_fd, curredir->new_fd);
		close(curredir->old_fd);
	}
	return (1);
}

int	open_files_setup(t_io *curredir)
{
	char	*fnmdlm;
	//int		hdfdpipe[2];

	fnmdlm = curredir->fname_delim;
	if (curredir->tredirect == fd2file_trunc)
		curredir->old_fd = open(fnmdlm, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	else if (curredir->tredirect == fd2file_append)
		curredir->old_fd = open(fnmdlm, O_WRONLY | O_CREAT | O_APPEND, 0666);
	else if (curredir->tredirect == file2fd)
		curredir->old_fd = open(fnmdlm, O_RDONLY);
	else if (curredir->tredirect == heredoc2fd)
		return (1);
	/*
	{
		if (pipe(hdfdpipe) == -1)
			return (perror("pipe"), 0);
		if (get_heredoc(hdfdpipe[1], fnmdlm) == 0)
			return(perror("heredoc???"), 0);//check returnvalues!;
		close (hdfdpipe[1]);
		dup2(hdfdpipe[0], curredir->new_fd);
		close(curredir->old_fd);
	}*/
	if (curredir->old_fd == -1)
		return(0);
	return (1);
}

int	child_setup(t_cd *pcd, t_cmd *curcmd, t_fdcl *fdcl, int *fdpipe)
{
	int		ret;
	t_io	*curredir;
	int		hdfdpipe[2];

	ret = 1;
	//first close pipes' ends that are not needed anymore and redirect pipe ends to stdin/stdout
	if (fdpipe[0] != -1)
		close(fdpipe[0]);
	if (fdcl->pre1[0] != -1)
	{
		dup2(fdcl->pre1[0], STDIN_FILENO);
		close(fdcl->pre1[0]);
	}
	if (fdpipe[1] != -1)
	{
		dup2(fdpipe[1], STDOUT_FILENO);
		close(fdpipe[1]);
	}

	//then heredocs in curcmd
	curredir = curcmd->iolst;
	while (curredir && ret != 0)
	{
		if (curredir->tredirect == heredoc2fd)
		{
			//create pipes for any heredocs in curcmd.
			if (pipe(hdfdpipe) == -1)
			{
				perror("pipe");
				exit (1);
			}
			//write into the pipe.
			if (get_heredoc(hdfdpipe[1], curredir->fname_delim) == 0)
			{
				perror("heredoc");//check returnvalues!;
				exit (1);
			}
			//close heredoc pipe write end and assign read end to old_fd.
			close (hdfdpipe[1]);
			curredir->old_fd = hdfdpipe[0];
		}
		curredir = curredir->next;
	}

	//then open all files until open fails, or all redirections are finished.
	curredir = curcmd->iolst;
	while (curredir && (ret != 0))
	{
		ret = open_files_setup(curredir);
		curredir = curredir->next;
	}
	//if open failed for one of the files, close all previously opened fds and exit
	if (ret == 0)
	{
		curredir = curcmd->iolst;
		while (curredir && curredir->old_fd != -1)
		{
			close (curredir->old_fd);
			curredir = curredir->next;
		}
		perror(curredir->fname_delim);
		exit(1); //msh_error?
	}
	//then set all redirections.
	curredir = curcmd->iolst;
	while (curredir)
	{
		if (redirections_setup(curredir) == 0)
		{
			perror(curcmd->iolst->fname_delim);
			exit(1); //msh_error?
		}
		curredir = curredir->next;
	}
	if (ft_is_builtin(curcmd->args[0]))
	{
		msh_builtins(pcd, curcmd->args);
		exit(0);
	}
	else
	{
		execve(curcmd->args[0], curcmd->args, environ);//environ is mo longer called that?
		perror(curcmd->args[0]); //perror what? wrong filename or options?
		exit (127);
	}
}

int	parent_setup(t_cd *pcd, t_cmd *curcmd, int *pids, int i)
{
	static t_fdcl	fdcl = {{-1,-1}, {-1, -1}};
	int				fdpipe[2] = {-1, -1};

	//if heredocs, all must be stored in parent in a char *buffer[N]
	if (fdcl.pre2[0] != -1)
		close(fdcl.pre2[0]);
	if (fdcl.pre1[1] != -1)
		close(fdcl.pre1[1]);
	if (curcmd->ctrlchr == '|')
	{
		if (pipe(fdpipe) == -1)
			return (perror("pipe"), 1);
	}
	/*if (curcmd->next || curcmd->iolst)
	{
		pids[i] = fork();
		if (pids[i] == -1)
		return (perror("fork"), 1);
	}*/
	pids[i] = fork();
	if (pids[i] == -1)
		return (perror("fork"), 1);
	if (pids[i] == 0)
	{
		child_setup(pcd, curcmd, &fdcl, fdpipe);
		exit(1);
		/*	return (1);
		else
			return(perror(curcmd->args[0]), 0); //this perror is wrong.*/
	}
	fdcl.pre2[0] = fdcl.pre1[0];
	fdcl.pre2[1] = fdcl.pre1[1];
	fdcl.pre1[0] = fdpipe[0];
	fdcl.pre1[1] = fdpipe[1];
	return (1);
}