/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 11:35:05 by rhaas             #+#    #+#             */
/*   Updated: 2025/11/04 18:52:15 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "msh_builtins.h"
#include "msh_compile_cmds.h"

#include "libft.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <unistd.h>

volatile sig_atomic_t g_signal_received = 0;

int	ms_init(t_cd *pcd)
{
	pcd->envrsvd = 0;
	pcd->msh_env = environ;
	check_cpy_env(pcd, true);
	pcd->exit_status = 42;
	pcd->interactive = isatty(0);
	pcd->fdscript = -1;
	pcd->exit = false;
	pcd->default_sqed = (t_sqed){.q = "\"\'", .esc = '\\',
		.blank = " \t", .metachr = "|<>\n", .ifs = " \t\n",
		.expnd = '$', .dict = &msh_dict, .dict_data = pcd,
		.validchar = &msh_isvalidvarchar, .validchar_data = (bool *) NULL};
	pcd->mshid = "msh";

	//signals
	if (pcd->interactive)
	{
		setup_signal(SIGINT, sigint_handler);
		//setup_signal(SIGQUIT, sigint_handler); this one should be the one for interrupting ongoing process like cat waiting for input
		setup_signal(SIGQUIT, SIG_IGN);
	}
	
	return (1);
}

int	ms_cleanup(t_cd *pcd)
{
	rl_clear_history();
	if (pcd->fdscript != -1)
		close(pcd->fdscript);
	rh_free_simple_array((void **)pcd->msh_env);
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
