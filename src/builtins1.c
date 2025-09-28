/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 19:23:31 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/25 20:13:42 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <limits.h>
#include <stdlib.h>

static int	msbi_echo(t_cd *pcd, char **args)
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
	if (newline && ret >= 0)
		ret = ft_printf("%c", '\n');
	pcd->exit_status = 0;
	return (ret >= 0);
}

static int	msbi_exit(t_cd *pcd, char **args)
{
	int	success;

	if (args[1])
	{
		pcd->exit_status = ft_atoll_check_input(args[1], INT_MAX, &success);
		if (success < (int)ft_strlen(args[1]))
			ft_printf("Dummy str: bash: exit: 82949672970920399492: "
				"numeric argument required");
		else
			pcd->exit_status %= 256;
	}
	if (pcd->interactive)
		ft_printf("exit\n");
	pcd->exit = true;
	return (1);
}

int	ms_builtins(t_cd *pcd, char **const args)
{
	if (!ft_strcmp(args[0], "echo"))
		return (msbi_echo(pcd, args));
	else if (!ft_strcmp(args[0], "exit"))
		return (msbi_exit(pcd, args));
	else
		return (0);
}
