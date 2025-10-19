/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 11:33:10 by rhaas             #+#    #+#             */
/*   Updated: 2025/10/19 18:40:27 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"

# include <stdbool.h>

extern char **environ;

typedef struct s_minishell_core_data
{
	char	*mshid;
	size_t	envrsvd;
	int		exit_status;
	bool	interactive;
	int		fdscript;
	bool	exit;
	t_sqed	default_sqed;
}	t_cd;

typedef enum e_redirect
{
	fd2file_trunc,
	fd2file_append,
	file2fd,
	heredoc2fd,
	// fd2fd,
	rd_default,
}	t_rd;

typedef struct s_io
{
	t_rd		tredirect;
	char		*fname_delim; // initialize to NULL?
	int			new_fd;
	int			old_fd;// initialize to -1? //either to store filename's fd, or store >&[3]
	struct s_io	*next;
}	t_io;

typedef struct s_cmdarg
{
	char			*arg;
	struct s_cmdarg	*next;
}	t_cmdarg;

typedef struct s_cmd
{
	int				stdio[3];//if it's for storing original stdio then t_stdio (see below) is redundant.
	t_io			*iolst;
	t_io			*curio;
	char			**args;
	char			ctrlchr; //('|')
	struct s_cmd	*next; //if linked list;
}	t_cmd;

typedef struct s_stdio //for storing original stdio. Haven't used it yet.
{
	int	stdin;
	int	stdout;
}	t_stdio;

int		get_cmdline_add_history(t_cd *pcd, char **pcmdline);
int		process_cmdline(t_cd *pcd, char const *const in);
int		setup_stdin(t_cd *pcd, int const argc, char **const argv);
size_t	get_envsize(char **const env);
size_t	check_cpy_env(char ***const oldenv, size_t * const penvreserved,
	bool free_oldenv);

char	**ft_env_paths(size_t *len);
char	*compose_check_cmd(t_cd *pcd, t_cmd *curcmd, char **epaths, size_t epaths_len);
//static int	compose_check_path(char **path, char *cmdexe, char *pathstr, size_t bufferlen);
//static int	access_check_path(char *pathstr);
//static int	free_splitstrs(char **split);

#endif // MINISHELL_H
