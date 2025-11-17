/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 11:33:10 by rhaas             #+#    #+#             */
/*   Updated: 2025/11/17 17:16:21 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include "msh_divdefs.h"

# include <stdbool.h>
# include <signal.h>
# include <unistd.h>

extern char	**environ;

extern volatile
sig_atomic_t g_signal_received;

typedef struct s_minishell_core_data
{
	char	**msh_env;
	size_t	envrsvd;
	int		exit_status;
	bool	interactive;
	t_pt	pt;
	int		fdscript;
	bool	exit;
	t_sqed	default_sqed;
}	t_cd;

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
	char			*path;
	size_t			bufferlen;
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

typedef struct s_signal_action_old
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;
	bool				sig_modif;
}	t_sa;

//input
int		get_cmdline_add_history(t_cd *pcd, char **pcmdline);
int		process_cmdline(t_cd *pcd);
void	msh_syntaxerror(t_cd *pcd, char const *const token,
			char const *const cmdline);
int		setup_stdin(t_cd *pcd, int const argc, char **const argv);
size_t	get_envsize(char **const env);
size_t	check_cpy_env(t_cd *pcd, bool env_init);
int		count_cmds(t_cmd *curcmd);
int		ft_is_builtin(char *cmd);
void	clear_fdinput(int fd);

//process
//static int	reset_stdiofds(t_cmd *curcmd);
//static int	execute_builtin(t_cd *pcd, t_cmd *cmd);
//static int	process_single_cmd(t_cd *pcd, t_cmd *cmd,
	// t_cmd_ctx *ctx, int *idx);
int		process_all_cmds(t_cd *pcd, t_cmd_ctx *ctx);
int		init_cmd_ctx(t_cmd_ctx *ctx, t_cd *pcd);
int		wait_child_processes(t_cd *pcd, int *pids, int ncmds);

//paths
char	**ft_env_paths(t_cd *pcd, size_t *len);
int		compose_check_cmd(t_cd *pcd, t_cmd *curcmd, t_cmd_ctx *ctx, char **path_noaccess);
//static int	access_check_path(char *pathstr);
int		free_splitstrs(char **split);
int		compose_check_cmd_get_path(t_cd *pcd, t_cmd_ctx *ctx, t_cmd *cmd, char **path_noaccess);
void	cmd_no_permissions(t_cd *pcd, t_cmd *cmd, bool *flag, char **path_noaccess);

//setup_exec
//int		parent_setup(t_cd *pcd, t_cmd *curcmd, int *pids, int i);
int		parent_setup(t_cd *pcd, t_cmd *curcmd, t_cmd_ctx *ctx, int i);
void	parent_reset_after_fork(t_fdcl *fdcl, int *fdpipe, t_io *curredir);
//int		child_setup(t_cd *pcd, t_cmd *curcmd, t_fdcl *fdcl, int *fdpipe);
int		child_setup(t_cd *pcd, t_cmd *curcmd, t_fdcl *fdcl, int *fdpipe, t_cmd_ctx *ctx);
int		prepare_external_cmd(t_cd *pcd, t_cmd *cmd, t_cmd_ctx *ctx, char **path_noaccess);
int		inchild_pipe_setup(t_fdcl *fdcl, int *fdpipe);
int		setup_redirections(t_io *iolst);
int		open_files_setup(t_io *curredir);
int		fail_open_closefds(t_io *curredir);
int		apply_redirection(t_io *curredir);
int		child_cmd_execute(t_cd *pcd, t_cmd *curcmd);

//heredocs
// int		heredoc_prompt(int fd, char *limiter);
int		heredoc_prompt(t_cd *pcd, int fd, char *limiter);
//static int	terminal_safe_heredoc_prompt(int fd, char *limiter);
int		heredoc_setup(t_io *curredir, t_cd *pcd);
int		heredoc_setup_send_to_prompt(t_cd *pcd, t_io *curredir, t_sa *sa_old, int *hdfdpipe);
void	heredoc_setup_clean_pipe_fail(t_cd *pcd, t_sa *sa_old);

//signals...should add D_POSIX_C_SOURCE 200809L to CFLAGS in Makefile?
int		setup_signal(int signum, void (*handler)(int));
int		setup_signal_heredoc(struct sigaction *sa_old_int,
			struct sigaction *sa_old_quit, void (*handler)(int));
void	setup_signal_wait(struct sigaction *old_sigint);
void	sigint_handler(int sig);
void	sigint_handler_heredoc(int sig);
void	sigint_handler_wait(t_cd *pcd, int wstatus, int ncmds, int idx, int *sgntcnt);
void	reset_handlers_heredoc(struct sigaction *sa_old_int,
			struct sigaction *sa_old_quit);

#endif // MINISHELL_H