/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_compile_cmds.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:13:03 by rhaas             #+#    #+#             */
/*   Updated: 2025/11/24 15:01:09 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSH_COMPILE_CMDS_H
# define MSH_COMPILE_CMDS_H

# include "minishell.h"

# include <stdbool.h>

char	*msh_dict(char const *const key, void *dict_data);
bool	msh_isvalidvarchar(char const c, void *validchar_data);
t_cmd	*msh_compile_cmds(t_cd *pcd, t_sqed const sqed);
bool	msh_compile_cmds_get_cmdlines(t_cd *pcd, char **pcmdln, t_cs cs);
void	cleanup_cmds(t_cmd **pcmds_head);
void	cleanup_cls(t_cls *pcls);

#endif // MSH_COMPILE_CMDS_H
