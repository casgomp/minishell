/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 13:50:29 by pecastro          #+#    #+#             */
/*   Updated: 2025/10/19 18:40:32 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "msh_rmquotes_expand.h"
#include "libft.h"

#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct	s_fdcls
{
	int	pre1[2];
	int	pre2[2];
}	t_fdcl;

int	get_heredoc(int fd, char *limiter)
{
	char	*ret;
	char	*prompt;
	int		len;

	len = ft_strlen(limiter);
	prompt = "> ";
	while (1)
	{
		if (ft_putstr(prompt) == -1)
			return (0);
		ret = get_next_line(0);
		if (!ret)
			return (0);
		if (ft_strncmp(ret, limiter, len) == 0 && ret[len] == '\n')
			return (free(ret), 1);
		if (ft_putstr_fd(ret, fd) == -1) //ft_putstr_fd has void return. Should be int.
			return (free(ret), 0);
		free (ret);
	}
}

/*redirections_setup is repetitive....use something like:
int open_redir_file(const char *filename, int oflags, mode_t mode)
{
    int fd;

    if (oflags & O_CREAT)
        fd = open(filename, oflags, mode);
    else
        fd = open(filename, oflags);

    if (fd == -1)
        perror(file);

    return fd;
}*/
int	redirections_setup(t_cmd *curcmd)
{
	char	*fnmdlm;
	char	*delim;
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
		//same
	}
	else if (curcmd->iolst->tredirect == file2fd)
	{
		curcmd->iolst->old_fd = open(fnmdlm, O_RDONLY);
		//same
	}
	else if (curcmd->iolst->tredirect == heredoc2fd)
	{
		if (pipe(hdfdpipe) == -1)
			return (perror(hdfdpipe), 1);
		if (get_heredoc(hdfdpipe[1], fnmdlm) == 0)
			//return(perror("?"), 1);//check returnvalues!
		close (hdfdpipe[1]);
		dup2(hdfdpipe[0], curcmd->iolst->new_fd);
		close(curcmd->iolst->old_fd)
	}
	return (1);
}

int	child_setup(t_cmd *curcmd, t_fdcl *fdcl, int *fdpipe)
{
	int	ret;

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
	while (curcmd->iolst && (ret != 0))
		ret = redirections_setup(curcmd);
	if (ret == 0)
		return (0); //should have cd as parameter so i can assign cd->exit_status = 1
	execve(curcmd->args[0], curcmd->args, environ);
	perror(execve);
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
		if (pipe(fdpipe) == -1);
			return (perror(fdpipe), 1);
	}
	if (curcmd->next)
	{
		pids[i] = fork();
		if (pids[i] == -1)
		return (perror(pids[i]), 1);
	}
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