/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 13:50:29 by pecastro          #+#    #+#             */
/*   Updated: 2025/10/21 17:44:55 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "msh_rmquotes_expand.h"
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

int	redirections_setup(t_cmd *curcmd)
{
	char	*fnmdlm;
	int		hdfdpipe[2];

	fnmdlm = curcmd->iolst->fname_delim;
	if (curcmd->iolst->tredirect == fd2file_trunc)
	{
		curcmd->iolst->old_fd = open(fnmdlm, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		if (curcmd->iolst->old_fd == -1)
			return (perror(fnmdlm), 0);
		dup2(curcmd->iolst->old_fd, curcmd->iolst->new_fd);//also check dup2 return and perror?
		close(curcmd->iolst->old_fd);
	}
	else if (curcmd->iolst->tredirect == fd2file_append)
	{
		curcmd->iolst->old_fd = open(fnmdlm, O_WRONLY | O_CREAT | O_APPEND, 0666);
		if (curcmd->iolst->old_fd == -1)
			return (perror(fnmdlm), 0);
	}
	else if (curcmd->iolst->tredirect == file2fd)
	{
		curcmd->iolst->old_fd = open(fnmdlm, O_RDONLY);
		if (curcmd->iolst->old_fd == -1)
			return (perror(fnmdlm), 0);
	}
	else if (curcmd->iolst->tredirect == heredoc2fd)
	{
		if (pipe(hdfdpipe) == -1)
			return (perror("pipe"), 1);
		if (get_heredoc(hdfdpipe[1], fnmdlm) == 0)
			//return(perror("?"), 1);//check returnvalues!;
		close (hdfdpipe[1]);
		dup2(hdfdpipe[0], curcmd->iolst->new_fd);
		close(curcmd->iolst->old_fd);
	}
	return (1);
}

int	child_setup(t_cmd *curcmd, t_fdcl *fdcl, int *fdpipe)
{
	int	ret;

	ret = 1;
	ft_printf("child_setup: fdpipe[0]= %d\n", fdpipe[0]);//debugging
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
	while (curcmd->iolst && (ret != 0))
		ret = redirections_setup(curcmd);
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
	ft_printf("parent_setup: i = %d\n", i);//debugging
	if (fdcl.pre2[0] != -1)
		close(fdcl.pre2[0]);
	if (fdcl.pre1[1] != -1)
		close(fdcl.pre1[1]);
	if (curcmd->ctrlchr == '|')
	{
		if (pipe(fdpipe) == -1)
			return (perror("pipe"), 1);
	}
	if (curcmd->next || curcmd->iolst)
	{
		ft_printf("entered loop!\n");//debugging
		pids[i] = fork();
		if (pids[i] == -1)
		return (perror("fork"), 1);
	}
	ft_printf("parent_setup: pids[%d]= %d\n", i, pids[i]);//debugging
	if (pids[i] == 0)
	{
		ft_printf("inside child!\n");//debugging
		child_setup(curcmd, &fdcl, fdpipe);
		return(perror(curcmd->args[0]),1 );//check return values.
	}
	fdcl.pre2[0] = fdcl.pre1[0];
	fdcl.pre2[1] = fdcl.pre1[1];
	fdcl.pre1[0] = fdpipe[0];
	fdcl.pre1[1] = fdpipe[1];
	return (1);
}