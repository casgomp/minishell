/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 10:38:42 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/03 09:47:19 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

typedef struct s_fs
{
	int		stdin;
	int		stdout;
	char	*f1name;
	char	*f2name;
	int		file1;
	int		file2;
	int		pipefd[2];
}	t_fds;

int	run_cmd(char *args);

#endif // PIPEX_H