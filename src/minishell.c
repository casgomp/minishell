/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 11:35:05 by rhaas             #+#    #+#             */
/*   Updated: 2025/11/17 17:16:29 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "msh_builtins.h"
#include "msh_compile_cmds.h"

#include "libft.h"

#include <readline/readline.h>
#include <stdlib.h>
#include <unistd.h>

volatile	sig_atomic_t
	g_signal_received = 0;

int	ms_init(t_cd *pcd)
{
	pcd->envrsvd = 0;
	pcd->msh_env = environ;
	check_cpy_env(pcd, true);
	pcd->exit_status = 0;
	pcd->interactive = isatty(0);
	pcd->pt = (t_pt)pt_std;
	pcd->fdscript = -1;
	pcd->exit = false;
	pcd->default_sqed = (t_sqed){.q = "\"\'", .rmqe = false, .esc = '\\',
		.lncmt = "# \t|<>\n;", .blank = " \t", .soloblnk = false,
		.metachr = "|<>\n;", .ifs = " \t\n", .expnd = '$',
		.dict = &msh_dict, .dict_data = pcd,
		.validchar = &msh_isvalidvarchar, .validchar_data = (bool *) NULL};
	if (pcd->interactive)
	{
		setup_signal(SIGINT, sigint_handler);
		setup_signal(SIGQUIT, SIG_IGN);
	}
	return (1);
}

int	ms_cleanup(t_cd *pcd)
{
	rl_clear_history();
	msh_compile_cmds(pcd, pcd->default_sqed, (char **) NULL);
	if (pcd->fdscript != -1)
		close(pcd->fdscript);
	rh_free_simple_array((void ***)&(pcd->msh_env));
	return (1);
}

int	main(int argc, char *argv[])
{
	t_cd	cd;

	ms_init(&cd);
	setup_stdin(&cd, argc, argv);
	while (true && !cd.exit)
	{
		process_cmdline(&cd);
	}
	ms_cleanup(&cd);
	return (cd.exit_status);
}
