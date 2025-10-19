/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 11:35:05 by rhaas             #+#    #+#             */
/*   Updated: 2025/10/19 18:40:25 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "msh_builtins.h"
#include "msh_rmquotes_expand.h"

#include "libft.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <unistd.h>

int	ms_init(t_cd *pcd)
{
	// set envvar 'SHELL' to sth like 'minishell' here in init
	// ---> no don't, bash also doesn't do it.
	//      If it uses "bash: " at the beginning of error msgs an the like,
	//      it is just hardcoded
	pcd->envrsvd = 0;
	check_cpy_env(&environ, &(pcd->envrsvd), true);
	pcd->exit_status = 42;
	pcd->interactive = isatty(0);
	pcd->fdscript = -1;
	pcd->exit = false;
	pcd->default_sqed = (t_sqed){.q = "\"\'", .esc = '\\',
		.blank = " \t", .metachr = "|<>",
		.expnd = '$', .dict = &msh_dict, .dict_data = pcd,
		.validchar = &msh_isvalidvarchar, .validchar_data = (bool *) NULL};
	pcd->mshid = "msh";
	return (1);
}

int	ms_cleanup(t_cd *pcd)
{
	rl_clear_history();
	if (pcd->fdscript != -1)
		close(pcd->fdscript);
	// clear env if ptr changed!
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
