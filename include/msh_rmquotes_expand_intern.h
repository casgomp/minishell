/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_rmquotes_expand_intern.h                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:13:03 by rhaas             #+#    #+#             */
/*   Updated: 2025/10/20 10:15:55 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSH_RMQUOTES_EXPAND_INTERN_H
# define MSH_RMQUOTES_EXPAND_INTERN_H

// # include "msh_rmquotes_expand.h"
# include "minishell.h"

typedef struct s_tokens_cmds_arg
{
	char		**tokens;
	t_cmd		*cmdlst;
	t_cmd		*curcmd;
	t_cmdarg	*arglst;
	t_cmdarg	*curarg;
	int			num_args;
}	t_tca;

int	set_curcmd(t_tca *ptca);
int	get_iolstelem(t_tca *ptca);
int	get_curarg(t_tca *ptca);
int	convert_arglst(t_tca *ptca);
int	cleanup_arglst(t_tca *ptca);
int	cleanup_tca(t_tca *ptca);

#endif // MSH_RMQUOTES_EXPAND_INTERN_H
