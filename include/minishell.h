/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 11:33:10 by rhaas             #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/09/28 16:01:05 by pecastro         ###   ########.fr       */
=======
/*   Updated: 2025/09/30 19:19:53 by rhaas            ###   ########.fr       */
>>>>>>> core_structure
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"

# include <stdbool.h>

extern char **environ;

typedef struct s_minishell_core_data
{
	char	**env;
	size_t	envsz;
	int		exit_status;
	bool	interactive;
	int		fdscript;
	bool	exit;
	t_sqed	default_sqed;
}	t_cd;

typedef struct s_fs
{
	int	stdin;
	int	stdout;
}	t_fds;

int		get_cmdline_add_history(t_cd *pcd, char **pcmdline);
int		ms_builtins(t_cd *pcd, char **const args);
int		process_cmdline(t_cd *pcd, char const *const in);
int		setup_stdin(t_cd *pcd, int const argc, char **const argv);
size_t	envsize(char **const env);
char	**check_cpy_env(char **oldenv, size_t *pexternenvsz, bool free_oldenv);

#endif // MINISHELL_H
