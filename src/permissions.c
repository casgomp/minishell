/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   permissions.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 16:41:08 by pecastro          #+#    #+#             */
/*   Updated: 2025/11/20 18:16:29 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "msh_builtins.h"
#include "libft.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

void	cmd_no_permissions(t_cd *pcd, t_cmd *cmd, bool *flag, t_cmd_ctx *ctx)
{
	*flag = 1;
	ctx->path_noaccess = ft_strdup(cmd->path);
	if (!ctx->parpth)
		pcd->exit_status = 126;
}

void	fail_stat_exit(t_cd *pcd, t_cmd_ctx *ctx)
{
	if (!ctx->parpth)
		pcd->exit_status = 127;
}
