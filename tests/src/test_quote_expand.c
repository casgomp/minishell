/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_quote_expand.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 11:35:05 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/25 18:40:19 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tests.h"
#include "ms_rmquotes_expand.h"
#include "libft.h"

#include <stdlib.h>
#include <stdio.h>

typedef struct testcase
{
	char *in;
	char *out;
	char *shellout;
}	t_tstcs;

int test_quote_expand()
{
	t_tstcs tests[] = {
	/*  0 */ {.in="\"a\\\"x\\\"b\"", },
	/*  1 */ {.in="\"ax\\\"b\"", },
	/*  2 */ {.in="\\\'a\\\"x\\\"b\\\'", },
	/*  3 */ {.in="a-$SHELL-b", },
	/*  4 */ {.in="a-\\$SHELL-b", },	
	/*  5 */ {.in="a-'$SHELL'-b", },
	/*  6 */ {.in="a-\\'$SHELL\\'-b", },		
	/*  7 */ {.in="\"a-'$SHELL'-b\"", },
	/*  8 */ {.in="a-$SHED3-b", },
	/*  9 */ {.in="a-$SHELL-b-'$SHELL'-c-", },
	/* 10 */ {.in="\"a-$SHELL-b-'$SHELL'-c-\"", },
	/* 11 */ {.in="\"a-$3SHELL-b-'$SHELL'-c-\"", },
	/* 12 */ {.in="\"a-$<-b\"", },
	/* 13 */ {.in="\"a-$A<-b\"", },
	/*eof */ {.in=NULL},
	};

	for (int i=0; tests[i].in; ++i)
	{
		printf("Test %2d: %s\n", i, tests[i].in);

		bool firstchar;
		tests[i].out = rh_rmqed(tests[i].in,
			(t_qed){.q="\"\'", .esc='\\', .expnd='$', .dict=ms_dict, .dict_data=NULL,
				.validchar=ms_isvalidvarchar, .validchar_data=&firstchar });
		printf("%8s %s\n", "--> :", tests[i].out);

		char cmdstr[128];
		sprintf(cmdstr, "echo -n %s", tests[i].in);
		FILE *shell = popen(cmdstr, "r");
		tests[i].shellout = NULL;
		size_t length = 0;
		getline(&tests[i].shellout, &length, shell);
		pclose(shell);
		printf("%8s %s\n", "echo :", tests[i].shellout);

		printf("%8s %s\n\n", "Status :", test_result_str(tests[i].out, tests[i].shellout, 1));
	}

	// Cleanup
	for (int i=0; tests[i].in; ++i)
	{
		if (tests[i].out)
			free(tests[i].out);
		if (tests[i].shellout)
			free(tests[i].shellout);
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
