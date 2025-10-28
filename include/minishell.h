/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 11:33:10 by rhaas             #+#    #+#             */
/*   Updated: 2025/10/28 13:46:49 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"

# include <stdbool.h>

extern char	**environ;
# define ENV_XTRA_ELEMS 8

typedef struct s_minishell_core_data
{
	char	*mshid;
	char	**msh_env;
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
	// for execve. args[0] will also later be concatenated to an absolute path.
	// Rapha: better leave args[0] alone to preserve the original state
	//        and use a separate var for the full path+executable
	char			**args;
	//('|') how is a pipe parsed (quoted/unquoted) and stored in struct?
	// Rapha: like this: ctrlchr == '|'
	char			ctrlchr;
	struct s_cmd	*next; //if linked list;
}	t_cmd;

// // See comment above in t_cmd: Rapha thinks we don't need it,
// // bc the parent will never mess with it's stdio
// // for storing original stdio. Haven't used it yet.
// typedef struct s_stdio
// {
// 	int	stdin;
// 	int	stdout;
// }	t_stdio;

int		get_cmdline_add_history(t_cd *pcd, char **pcmdline);
int		process_cmdline(t_cd *pcd, char const *const in);
int		setup_stdin(t_cd *pcd, int const argc, char **const argv);

#endif // MINISHELL_H
