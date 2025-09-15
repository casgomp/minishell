/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_processes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pecastro <pecastro@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 14:05:11 by pecastro          #+#    #+#             */
/*   Updated: 2025/08/31 12:04:27 by pecastro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "pipex.h"

int	ft_processes(t_pipex *pipex)
{
	int		wstatus[2];
	int		status_code;

	if (ft_children(pipex) == 1)
		return (1);
	close (pipex->fd_f[0]);
	close (pipex->fd_f[1]);
	close (pipex->fd[0]);
	close (pipex->fd[1]);
	status_code = 0;
	waitpid (pipex->pid[0], &wstatus[0], 0);
	if (WIFEXITED(wstatus[0]))
		status_code = WEXITSTATUS(wstatus[0]);
	waitpid (pipex->pid[1], &wstatus[1], 0);
	if (WIFEXITED(wstatus[1]))
		status_code = WEXITSTATUS(wstatus[1]);
	return (status_code);
}

int	ft_children(t_pipex *pipex)
{
	pipex->fd_f[0] = open(pipex->file[0], O_RDONLY);
	if (pipex->fd_f[0] == -1)
		return (perror("Error: open()"), 1);
	pipex->fd_f[1] = open(pipex->file[1], O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (pipex->fd_f[1] == -1)
		return (perror("Error: open()"), 1);
	if (ft_access_paths(pipex) > 0)
		return (1);
	if (pipe(pipex->fd) == -1)
		return (perror("Error: pipe()"), 1);
	pipex->pid[0] = fork();
	if (pipex->pid[0] == -1)
		return (perror("Error: fork()"), 1);
	if (pipex->pid[0] == 0)
		ft_process1(pipex);
	pipex->pid[1] = fork();
	if (pipex->pid[1] == -1)
		return (perror("Error: fork()"), 1);
	if (pipex->pid[1] == 0)
		ft_process2(pipex);
	return (0);
}

void	ft_process1(t_pipex *pipex)
{
	dup2(pipex->fd_f[0], STDIN_FILENO);
	dup2(pipex->fd[1], STDOUT_FILENO);
	close(pipex->fd_f[0]);
	close(pipex->fd_f[1]);
	close(pipex->fd[0]);
	close(pipex->fd[1]);
	printf("\n");
	execve(pipex->path[0], pipex->cmd[0], environ);
	perror("Error: execve()");
	exit (1);
}

void	ft_process2(t_pipex *pipex)
{
	dup2(pipex->fd[0], STDIN_FILENO);
	dup2(pipex->fd_f[1], STDOUT_FILENO);
	close(pipex->fd_f[0]);
	close(pipex->fd_f[1]);
	close (pipex->fd[0]);
	close (pipex->fd[1]);
	execve (pipex->path[1], pipex->cmd[1], environ);
	perror("Error: execve()");
	exit(1);
}
