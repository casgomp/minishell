/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 11:33:10 by rhaas             #+#    #+#             */
/*   Updated: 2025/10/25 18:01:16 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"

# include <stdbool.h>

extern char	**environ;

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
	char		*fname_delim;
	int			new_fd;
	int			old_fd;
	struct s_io	*next;
}	t_io;

typedef struct s_cmdarg
{
	char			*arg;
	struct s_cmdarg	*next;
}	t_cmdarg;

typedef struct s_cmd
{
	// Rapha: after talking about how bash handles pipes and redirects
	// (always done in the child) we don't need to save and original states...
	//if it's for storing orig. stdio then t_stdio (see below) is redundant.
	// int				stdio[3];
	t_io			*iolst;
	t_io			*curio;
	char			**args; //for execve. args[0] will also later be concatenated to an absolute path.
	char			ctrlchr; //('|') how is a pipe parsed (quoted/unquoted) and stored in struct?
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
//additions to input.c:
int	count_children(t_cmd *curcmd);

char	**ft_env_paths(size_t *len);
char	*compose_check_cmd(t_cd *pcd, t_cmd *curcmd, char **epaths, size_t epaths_len);
//static int	compose_check_path(char **path, char *cmdexe, char *pathstr, size_t bufferlen);
//static int	access_check_path(char *pathstr);
//static int	free_splitstrs(char **split);

int		parent_setup(t_cmd *curcmd, int *pids, int i);
int		child_setup(t_cmd *curcmd, t_fdcl *fdcl, int *fdpipe);
int		redirections_setup(t_io *curredir);
int		get_heredoc(int fd, char *limiter);

#endif // MINISHELL_H
