/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pecastro <pecastro@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 14:03:21 by pecastro          #+#    #+#             */
/*   Updated: 2025/08/31 12:04:10 by pecastro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "pipex.h" 

int	main(int argc, char **argv)
{
	t_pipex	pipex;
	int		i;
	int		status_code;

	if (argc != 5)
		return (ft_dprintf(STDERR_FILENO, "./pipex file1 cmd1 cmd2 file2\n"), 1);
	pipex.file[0] = argv[1];
	pipex.file[1] = argv[argc - 1];
	if (!ft_contains_words(argv[2]) || !ft_contains_words(argv[3]))
		return (ft_dprintf(STDERR_FILENO, "Error: empty command\n"), 1);
	i = 0;
	while (i < argc - 3)
	{
		pipex.cmd[i] = ft_split(argv[i + 2], ' ');
		pipex.cmd_rel[i] = ft_strjoin("/", pipex.cmd[i][0]);
		pipex.complete_paths[i] = ft_complete_paths(pipex.cmd_rel[i]);
		if (!pipex.complete_paths[i])
			return (ft_free(i, &pipex), 1);
		i ++;
	}
	status_code = ft_processes(&pipex);
	return (ft_free(i - 1, &pipex), status_code);
}
