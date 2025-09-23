/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 11:35:05 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/23 13:56:40 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <unistd.h>

int ms_init(t_cd *pcd)
{
	pcd->exit_status = 42;
	pcd->interactive = isatty(0);
	pcd->fdscript = -1;
	pcd->exit = false;
	pcd->default_qes = (t_qes){.splt = " \t", .specialsplt="", .q="\"\'", .esc='\\'};
	pcd->default_qed = (t_qed){.dict=&ms_dict, .data=pcd, .esc='\0', .expnd='$', .q="\"\'"};
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
	while (true)
	{
		get_cmdline_add_history(&cd, &in);
		process_cmdline(&cd, in);
		free(in);
		if (cd.exit)
			break;
	}
	ms_cleanup(&cd);
	return (EXIT_SUCCESS);
}
