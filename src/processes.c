/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 13:50:29 by pecastro          #+#    #+#             */
/*   Updated: 2025/10/28 18:15:33 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
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
	int		hdfdpipe[2];

	fnmdlm = curredir->fname_delim;
	if (curredir->tredirect == fd2file_trunc)
	{
		curredir->old_fd = open(fnmdlm, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		if (curredir->old_fd == -1)
			return (perror(fnmdlm), 0);
		dup2(curredir->old_fd, curredir->new_fd);//also check dup2 return and perror?
		close(curredir->old_fd);
	}
	else if (curredir->tredirect == fd2file_append)
	{
		curredir->old_fd = open(fnmdlm, O_WRONLY | O_CREAT | O_APPEND, 0666);
		if (curredir->old_fd == -1)
			return (perror(fnmdlm), 0);
		dup2(curredir->old_fd, curredir->new_fd);//also check dup2 return and perror?
		close(curredir->old_fd);
	}
	else if (curredir->tredirect == file2fd)
	{
		curredir->old_fd = open(fnmdlm, O_RDONLY);
		if (curredir->old_fd == -1)
			return (perror(fnmdlm), 0);
		dup2(curredir->old_fd, curredir->new_fd);//also check dup2 return and perror?
		close(curredir->old_fd);
	}
	else if (curredir->tredirect == heredoc2fd)
	{
		if (pipe(hdfdpipe) == -1)
			return (perror("pipe"), 0);
		if (get_heredoc(hdfdpipe[1], fnmdlm) == 0)
			return(perror("heredoc???"), 0);//check returnvalues!;
		close (hdfdpipe[1]);
		dup2(hdfdpipe[0], curredir->new_fd);
		close(curredir->old_fd);
	}
	return (1);
}

int	child_setup(t_cmd *curcmd, t_fdcl *fdcl, int *fdpipe)
{
	int		ret;
	t_io	*curredir;

	ret = 1;
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
	//should check_compose args here in child?.
	curredir = curcmd->iolst;
	while (curredir && (ret != 0))
	{
		ret = redirections_setup(curredir);
		curredir = curredir->next;
	}
	if (ret == 0)
		return (0); //should have cd as parameter so i can assign cd->exit_status = 1
	execve(curcmd->args[0], curcmd->args, environ);
	perror(curcmd->args[0]);//what? wrong filename or options?
	return (0);//and set exits status?
}

int	parent_setup(t_cmd *curcmd, int *pids, int i)
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
		child_setup(curcmd, &fdcl, fdpipe);
		return(perror(curcmd->args[0]),1 );//check return values.
	}
	fdcl.pre2[0] = fdcl.pre1[0];
	fdcl.pre2[1] = fdcl.pre1[1];
	fdcl.pre1[0] = fdpipe[0];
	fdcl.pre1[1] = fdpipe[1];
	return (1);
}