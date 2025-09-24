/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 19:23:31 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/24 14:13:34 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <stdlib.h>

static int	msbi_echo(char **args)
{
	int		ret;
	bool	newline;

	ret = 0;
	newline = true;
	while (*(++args) && !ft_strcmp(*args, "-n"))
		newline = false;
	--args;
	while (*(++args) && ret >= 0)
	{
		ret = ft_printf("%s ", *args);
	}
	if (ret > 0)
		ret = ft_printf("%c", '\b');
	if (newline && ret > 0)
		ret = ft_printf("%c", '\n');
	return (ret > 0);
}

static int	msbi_exit(t_cd *pcd)
{
	if (pcd->interactive)
		ft_printf("exit\n");
	pcd->exit = true;
	return (1);
}

int	ms_builtins(t_cd *pcd, char **const args)
{
	if (!ft_strcmp(args[0], "echo"))
		return (msbi_echo(args));
	else if (!ft_strcmp(args[0], "exit"))
		return (msbi_exit(pcd));
	else
		return (0);
}
