/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pecastro <pecastro@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 16:29:07 by pecastro          #+#    #+#             */
/*   Updated: 2025/08/31 12:01:51 by pecastro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef PIPEX_H
# define PIPEX_H

# include <errno.h>
# include <stdio.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <unistd.h>
# include "../lib/libft/include/libft.h"

typedef struct s_pipex
{
	char	*file[2];
	char	**cmd[2];
	char	*cmd_rel[2];
	char	**complete_paths[2];
	char	*path[2];
	int		fd[2];
	int		fd_f[2];
	int		pid[2];
}t_pipex;

extern char	**environ;

//main
int		main(int argc, char **argv);
//ft_paths
char	**ft_complete_paths(char *cmd);
char	**ft_paths_from_environ(void);
int		ft_access_paths(t_pipex *pipex);
char	ft_contains_words(char *str);
int		ft_size_arr(char **arr);
//ft_processes
int		ft_processes(t_pipex *pipex);
int		ft_children(t_pipex *pipex);
void	ft_process1(t_pipex *pipex);
void	ft_process2(t_pipex *pipex);
//ft_clean
void	ft_clean_arr_partial(int index, char **arr);
void	ft_clean_arr(char **arr);
void	ft_free(int index, t_pipex *pipex);

#endif
