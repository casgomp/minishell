/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 11:33:10 by rhaas             #+#    #+#             */
/*   Updated: 2025/11/06 19:50:57 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"

# include <stdbool.h>
# include <signal.h>

extern char	**environ;
# define ENV_XTRA_ELEMS 8

extern volatile sig_atomic_t g_signal_received;

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
	int				stdio[3];
	t_io			*iolst;
	t_io			*curio;
	char			**args;
	char			ctrlchr;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_cmd_ctx
{
	t_cmd	*cmds;
	char	**epaths;
	size_t	epaths_len;
	int		*pids;
	int		ncmds;
	bool	childless;
}	t_cmd_ctx;

typedef struct s_fdcl
{
	int	pre1[2];
	int	pre2[2];
}	t_fdcl;

//input
int		get_cmdline_add_history(t_cd *pcd, char **pcmdline);
int		process_cmdline(t_cd *pcd, char const *const in);
int		setup_stdin(t_cd *pcd, int const argc, char **const argv);
size_t	get_envsize(char **const env);
size_t	check_cpy_env(t_cd *pcd, bool env_init);
int		count_cmds(t_cmd *curcmd);
int		ft_is_builtin(char *cmd);

//process
//static int	reset_stdiofds(t_cmd *curcmd);
//static int	execute_builtin(t_cd *pcd, t_cmd *cmd);
//static void	prepare_external_cmd(t_cd *pcd, t_cmd *cmd, char **epaths, size_t epaths_len);
//static int	process_single_cmd(t_cd *pcd, t_cmd *cmd, t_cmd_ctx *ctx, int *idx);
int		process_all_cmds(t_cd *pcd, t_cmd_ctx *ctx);
int		init_cmd_ctx(t_cmd_ctx *ctx, t_cd *pcd, char const *in);
void	wait_child_processes(t_cd *pcd, int *pids, int ncmds);

//paths
char	**ft_env_paths(size_t *len);
char	*compose_check_cmd(t_cd *pcd, t_cmd *curcmd, char **epaths, size_t epaths_len);
//static int	compose_check_path(char **path, char *cmdexe, char *pathstr, size_t bufferlen);
//static int	access_check_path(char *pathstr);
int		free_splitstrs(char **split);

//setup_exec
int		parent_setup(t_cd *pcd, t_cmd *curcmd, int *pids, int i);
int		child_setup(t_cd *pcd, t_cmd *curcmd, t_fdcl *fdcl, int *fdpipe);
int		inchild_pipe_setup(t_fdcl *fdcl, int *fdpipe);
int		setup_redirections(t_io *iolst);
int		open_files_setup(t_io *curredir);
int		fail_open_closefds(t_io *curredir);
int		apply_redirection(t_io *curredir);
int		child_cmd_execute(t_cd *pcd, char **args);

//heredocs
int			heredoc_prompt(int fd, char *limiter);
//static int	terminal_safe_heredoc_prompt(int fd, char *limiter);
int			heredoc_setup(t_io *curredir);

//signals
int		setup_signal(int signum, void(*handler)(int));
void	sigint_handler(int sig);
int		setup_signal_heredoc(struct sigaction *sa, void(*handler)(int)); //should add D_POSIX_C_SOURCE 200809L to CFLAGS in Makefile?
void	reset_handlers_heredoc(struct sigaction *sa);
void	sigint_handler_heredoc(int sig);

#endif // MINISHELL_H