/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 11:35:05 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/15 20:38:21 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ms_rmquotes_expand.h"
#include "libft.h"

#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

int test_rhaas()
{
	char *tests[] = {
	/*  0 */ "a\"x\"b",
	/*  1 */ "\'a\"x\"b\'",
	/*  2 */ "a-$SHELL-b",
	/*  3 */ "a-\\$SHELL-b",		
	/*  4 */ "a-'$SHELL'-b",
	/*  5 */ "a-\\'$SHELL\\'-b",		
	/*  6 */ "\"a-'$SHELL'-b\"",
	/*  7 */ "a-$SHED3-b",
	/*  8 */ "a-$SHELL-b-'$SHELL'-c-",
	/*  9 */ "\"a-$SHELL-b-'$SHELL'-c-\"",
		NULL,
	};

	char *tmp;
	for (int i=0; tests[i]; ++i)
	{
		printf("Test %2d: %s\n", i, tests[i]);
		tmp = ms_rmquotes_expand(tests[i], (t_qed){"\"\'", '\\', '$', getenv});
		printf("   --> : %s\n", tmp);
		if (tmp)
			free(tmp);
	}
	return (1);
}

int	main(int argc, char *argv[])
{
	return (test_rhaas(), 0);
	(void)argc;
	(void)argv;
	char *in;
	while ((in = readline("<fancy prompt>$ ")))
	{
		if (in && *in != ' ')
			add_history(in);
		if (in && !ft_strncmp(in, "exit", 5))
		{
			free(in);
			break;
		}
		printf("Test io: %s\n", in);
		free(in);
	}
	rl_clear_history();
	clear_history();
	return (EXIT_SUCCESS);
}