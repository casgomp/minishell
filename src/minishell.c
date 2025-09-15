/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 11:35:05 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/15 20:02:57 by rhaas            ###   ########.fr       */
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
		"a\"x\"b",
		"a-$SHELL-b",
		"a-$SHED3-b",
		NULL,
	};

	for (int i=0; tests[i]; ++i)
	{
		printf("Test %d:\n", i);
		printf("OString: %s:\n", tests[i]);
		printf("PString: %s:\n", 
			ms_rmquotes_expand(tests[i], "\"\'", '\\'));
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