/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs_prompt.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 16:20:50 by pecastro          #+#    #+#             */
/*   Updated: 2025/11/25 15:02:56 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "msh_builtins.h"
#include "msh_compile_cmds.h"
#include "libft.h"

#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

static bool	heredoc_check_retval(char const *const ret,
	char const *const limiter)
{
	char	buf[512];

	if (!ret)
	{
		if (g_signal_received == SIGINT)
			return (false);
		ft_sprintf(buf, "here-document delimited by "
			"end-of-file (wanted `%s')", limiter);
		msh_error((char const *[]){"warning", buf, }, 2, -1);
		return (false);
	}
	return (true);
}

int	heredoc_prompt(t_cd *pcd, int fd, char *limiter)
{
	char	*cmdln;
	int		len;

	len = ft_strlen(limiter);
	while (true)
	{
		cmdln = NULL;
		msh_compile_cmds_get_cmdlines(pcd, &cmdln);
		if (g_signal_received == SIGINT)
			return (0);
		if ((!heredoc_check_retval(cmdln, limiter))
			|| (cmdln && ft_strncmp(cmdln, limiter, len) == 0
				&& cmdln[len] == '\0'))
			return (true);
		if (ft_dprintf(fd, "%s\n", cmdln) < 0)
			return (false);
	}
}
