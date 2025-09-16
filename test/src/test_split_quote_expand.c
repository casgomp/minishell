/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_split_quote_expand.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 11:35:05 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/16 16:05:56 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_rmquotes_expand.h"

#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

int test_quote_expand()
{
	char *tests[][2] = {
	/*  0 */ {"a\"x\"b", ""},
	/*  1 */ {"\'a\"x\"b\'", ""},
	/*  2 */ {"a-$SHELL-b", ""},
	/*  3 */ {"a-\\$SHELL-b", ""},	
	/*  4 */ {"a-'$SHELL'-b", ""},
	/*  5 */ {"a-\\'$SHELL\\'-b", ""},		
	/*  6 */ {"\"a-'$SHELL'-b\"", ""},
	/*  7 */ {"a-$SHED3-b", ""},
	/*  8 */ {"a-$SHELL-b-'$SHELL'-c-", ""},
	/*  9 */ {"\"a-$SHELL-b-'$SHELL'-c-\"", ""},
	/* 10 */ {"\"a-$3SHELL-b-'$SHELL'-c-\"", ""},
	/* 11 */ {"\"a-$<-b\"", ""},
	/* 12 */ {"\"a-$A<-b\"", ""},
		// NULL,
	};

	char *tmp;
	int len = sizeof(tests);
	for (int i=0; i < len/* tests[i] */; ++i)
	{
		printf("Test %2d: %s\n", i, tests[i][0]);
		tmp = ms_rmquotes_expand(tests[i][0], (t_qed){"\"\'", '\\', '$', getenv});
		printf("   --> : %s\n", tmp);
		if (tmp)
			free(tmp);
	}
	return (1);
}

int	main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	test_quote_expand();
	return (EXIT_SUCCESS);
}