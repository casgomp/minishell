/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 13:50:29 by pecastro          #+#    #+#             */
/*   Updated: 2025/11/06 19:08:30 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# define _POSIX_C_SOURCE 200809L
# include "minishell.h"
# include "msh_builtins.h"
# include "libft.h"

# include <fcntl.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <unistd.h>

int	child_cmd_execute(t_cd *pcd, char **args)
{
	struct sigaction sa;

	ft_memset(&sa, 0, sizeof(sa));
	if (ft_is_builtin(args[0]))
	{
		msh_builtins(pcd, args);
		exit(0);
	}
	else
	{
		if (args[0])
		{
			sa.sa_handler = SIG_DFL;
			sigemptyset(&sa.sa_mask);
			sigaction(SIGINT, &sa, NULL);
			sigaction(SIGQUIT, &sa, NULL);
			execve(args[0], args, environ);//environ is no longer called that?
			perror(args[0]);
		}
		exit (127);
	}
	return (1);
}

int	apply_redirection(t_io *curredir)
{
	dup2(curredir->old_fd, curredir->new_fd);
	close(curredir->old_fd);
	return (1);
}

int	fail_open_closefds(t_io *curredir)
{
	while (curredir && curredir->old_fd != -1)
	{
		close (curredir->old_fd);
		curredir = curredir->next;
	}
	perror(curredir->fname_delim);//msh_error?
	return (1);
}

int	open_files_setup(t_io *curredir)
{
	char	*fnmdlm;

	fnmdlm = curredir->fname_delim;
	if (curredir->tredirect == fd2file_trunc)
		curredir->old_fd = open(fnmdlm, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	else if (curredir->tredirect == fd2file_append)
		curredir->old_fd = open(fnmdlm, O_WRONLY | O_CREAT | O_APPEND, 0666);
	else if (curredir->tredirect == file2fd)
		curredir->old_fd = open(fnmdlm, O_RDONLY);
	else if (curredir->tredirect == heredoc2fd)
		return (1);
	if (curredir->old_fd == -1)
		return(0);
	return (1);
}

int	setup_redirections(t_io *iolst)
{
	t_io	*curredir;
	int		ret;

	curredir = iolst;
	ret = 1;
	while (curredir && ret != 0)
	{
		ret = open_files_setup(curredir);
		curredir = curredir->next;
	}
	if (ret == 0)
		return (fail_open_closefds(iolst), 0);
	curredir = iolst;
	while (curredir)
	{
		if (apply_redirection(curredir) == 0)
			return (perror(iolst->fname_delim), 0);
		curredir = curredir->next;
	}
	return (1);
}

int	inchild_pipe_setup(t_fdcl *fdcl, int *fdpipe)
{
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
	return (1);
}

int	child_setup(t_cd *pcd, t_cmd *curcmd, t_fdcl *fdcl, int *fdpipe)
{
	g_signal_received = 0;

	//inchild_pipe_setup(fdcl, fdpipe);
	if (!heredoc_setup(curcmd->iolst))
		exit (130);//this is wrong because heredoc setup can also fail in pipe creation (i.e. exit(1)).
	inchild_pipe_setup(fdcl, fdpipe);
	if (!setup_redirections(curcmd->iolst))
		exit (1);
	child_cmd_execute(pcd, curcmd->args);
	return (1);
}

int	parent_setup(t_cd *pcd, t_cmd *curcmd, int *pids, int i)
{
	static t_fdcl	fdcl = {{-1,-1}, {-1, -1}};
	int				fdpipe[2] = {-1, -1};

	if (fdcl.pre2[0] != -1)
		close(fdcl.pre2[0]);
	if (fdcl.pre1[1] != -1)
		close(fdcl.pre1[1]);
	if (curcmd->ctrlchr == '|')
	{
		if (pipe(fdpipe) == -1)
			return (perror("pipe"), 1);
	}
	pids[i] = fork();
	if (pids[i] == -1)
		return (perror("fork"), 1);
	if (pids[i] == 0)
	{
		child_setup(pcd, curcmd, &fdcl, fdpipe);
		exit(1);
	}
	if (fdcl.pre1[0] != -1)
		close(fdcl.pre1[0]);
	if (fdpipe[1] != -1)
		close(fdpipe[1]);
	fdcl.pre2[0] = fdcl.pre1[0];
	fdcl.pre2[1] = fdcl.pre1[1];
	fdcl.pre1[0] = fdpipe[0];
	fdcl.pre1[1] = fdpipe[1];
	return (1);
}