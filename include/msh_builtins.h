/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_builtins.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 19:17:00 by rhaas             #+#    #+#             */
/*   Updated: 2025/10/20 10:16:50 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSH_BUILTINS_H
# define MSH_BUILTINS_H

# include "minishell.h"

int		msh_builtins(t_cd *pcd, char **const args);

int		msh_env(void);
size_t	get_envsize(char **const env);
size_t	check_cpy_env(char ***const oldenv, size_t *const penvreserved,
			bool free_oldenv);
int		msh_export(t_cd *pcd, char **args);
int		msh_unset(char **args);

void	msh_error(char const **const errstrs, int nstrs, int errnum);
#endif // MSH_BUILTINS_H