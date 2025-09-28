/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 11:35:05 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/26 16:41:43 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ms_rmquotes_expand.h"
#include "libft.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <unistd.h>

int	ms_init(t_cd *pcd)
{
	// set envvar 'SHELL' to sth like 'minishell' here in init
	pcd->exit_status = 42;
	pcd->interactive = isatty(0);
	pcd->fdscript = -1;
	pcd->exit = false;
	pcd->default_sqed = (t_sqed){.q = "\"\'", .esc = '\\',
		.splt = " \t\n", .specialsplt = "",
		.expnd = '$', .dict = &ms_dict, .dict_data = pcd,
		.validchar = &ms_isvalidvarchar, .validchar_data = (bool *) NULL};
	return (1);
}

int	ms_cleanup(t_cd *pcd)
{
	rl_clear_history();
	if (pcd->fdscript != -1)
		close(pcd->fdscript);
	return (1);
}

int	main(int argc, char *argv[])
{
	char	*in;
	t_cd	cd;

	ms_init(&cd);
	setup_stdin(&cd, argc, argv);
	while (true && !cd.exit)
	{
		get_cmdline_add_history(&cd, &in);
		process_cmdline(&cd, in);
		free(in);
	}
	ms_cleanup(&cd);
	return (cd.exit_status);
}
