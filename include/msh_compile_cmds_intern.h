/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_compile_cmds_intern.h                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:13:03 by rhaas             #+#    #+#             */
/*   Updated: 2025/11/17 21:01:05 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSH_COMPILE_CMDS_INTERN_H
# define MSH_COMPILE_CMDS_INTERN_H

# include "minishell.h"

typedef struct s_tokens_cmds_arg
{
	char const	*tosplit;
	char		**tokens;
	t_cmd		*cmdlst;
	t_cmd		*curcmd;
	t_cmdarg	*arglst;
	t_cmdarg	*curarg;
	int			num_args;
}	t_tca;

int		set_curcmd(t_tca *ptca);
int		get_iolstelem(t_tca *ptca);
int		get_curarg(t_tca *ptca);
int		convert_arglst(t_tca *ptca);
int		cleanup_arglst(t_tca *ptca);
int		cleanup_tca(t_tca *ptca);
void	cleanup_iolst(t_io **piolst_head);
char	**msh_expand_wordsplit_token(char const *const token,
			t_sqed const *const psqed);
int		msh_process_metachar(t_cd *pcd, t_tca *ptca,
			int *ptokenidx, t_sqed const sqed);
t_cs	msh_check_cmds(t_cd *const pcd, t_cmd const *const cmds,
			char const *const cmdline);
bool	msh_check_cmdstatus_ok(t_cd *const pcd, int ret, t_cmd **pcmds);
bool	msh_process_ioredirector(t_cd *pcd, t_tca *ptca,
			int const	tokenidx, t_sqed const sqed);
void	msh_syntaxerror(t_cd *pcd, char const *const token,
			char const *const cmdline);

#endif // MSH_COMPILE_CMDS_INTERN_H
