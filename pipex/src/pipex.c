/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:37:27 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/04 17:07:21 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "libft.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static int	check_dup(int fd, int fdnew, char const *const src)
{
	if (fd == -1)
		return (0);
	if (dup2(fd, fdnew) == -1)
		return (perror(src), 0);
	close(fd);
	return (1);
}

static int	reset_stdiofds(t_fds const fds)
{
	dup2(fds.stdin, 0);
	dup2(fds.stdout, 1);
	close(fds.stdin);
	close(fds.stdout);
	return (1);
}

static void	setup_fds(t_fds *const pfds,
	char **const argv)
{
	pfds->stdin = dup(0);
	pfds->stdout = dup(1);
	pfds->f1name = argv[1];
	pfds->f2name = argv[4];
	pfds->file1 = open(pfds->f1name, O_RDONLY);
	if (pfds->file1 == -1)
		perror(pfds->f1name);
	pfds->file2 = open(pfds->f2name, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (pfds->file2 == -1)
		perror(pfds->f2name);
	if (pipe(pfds->pipefd) == -1)
		pfds->pipefd[1] = -1;
}

int	main(int argc, char *argv[])
{
	t_fds	fds;

	if (argc != 5)
		return (ft_printf("Usage: %s: file1 cmd1 cmd2 file2\n", argv[0]),
			EXIT_FAILURE);
	setup_fds(&fds, argv);
	if (!check_dup(fds.file1, 0, fds.f1name))
		close(fds.pipefd[1]);
	else if (check_dup(fds.pipefd[1], 1, "pipe"))
		run_cmd(argv[2]);
	if (!check_dup(fds.pipefd[0], 0, "pipe"))
		close (fds.file2);
	else if (check_dup(fds.file2, 1, fds.f2name)
		&& run_cmd(argv[3]))
		return (reset_stdiofds(fds), EXIT_SUCCESS);
	return (reset_stdiofds(fds), EXIT_FAILURE);
}
