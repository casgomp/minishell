/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_split.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 11:35:05 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/24 17:36:22 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tests.h"
#include "libft.h"
#include "ms_rmquotes_expand.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct testcase
{
	char *in;
	char **splt;
	char *splt2;
	char *shellout;
	bool native;
}	t_tstcs;

int test_split()
{
	t_tstcs tests[] = {
	/*  0 */ {.in="ein wort", .shellout=NULL},
	/*  1 */ {.in="ein|wort", .shellout="°ein°|°wort°"},
	/*  2 */ {.in="ein||wort", .shellout="°ein°||°wort°"},
	/*  3 */ {.in="ein ||wort", .shellout="°ein°||°wort°"},
	/*  4 */ {.in="ein |wort", .shellout="°ein°|°wort°"},
	/*  5 */ {.in="ein |   wort", .shellout="°ein°|°wort°"},
	/*  6 */ {.in="ein >>   wort", .shellout="°ein°>>°wort°"},
	/*  7 */ {.in="ein | | wort", .shellout="°ein°|°|°wort°"},
	/*  8 */ {.in="'ein wort' ohne \"hh\"", .shellout="°'ein wort'°ohne°\"hh\"°"},
	/*  9 */ {.in="\'ein wort\' ohne \"hh\"", .shellout=NULL},
	/*eof */ {.in=NULL},
	};

	t_qes qes = {.splt=" \t",
				.q="\"\'",
				.esc='\0',
				.specialsplt="|<>"};

	for (int i=0; tests[i].in; ++i)
	{
		printf("Test %2d   : %s\n", i, tests[i].in);

		tests[i].native = (tests[i].shellout == NULL);
		tests[i].splt = rh_split_quotes(tests[i].in, qes);
		if (!tests[i].splt)
		{
			printf("%p\n", tests[i].splt);
			continue;
		}
		tests[i].splt2 = malloc((256 + ft_strlen(tests[i].in))*(sizeof(char)));
		sprintf(tests[i].splt2, "°");
		int idx = 0;
		while (tests[i].splt[idx])
			sprintf(tests[i].splt2 + strlen(tests[i].splt2), "%s°", tests[i].splt[idx++]);
		printf("%11s %s\n", "--> :", tests[i].splt2);

		if (tests[i].native)
		{
			tests[i].splt2 = rh_rmqed(tests[i].splt2,
									(t_qed){.dict=NULL, .esc='\\', .expnd='\0', .q="\"\'"});

									char *cmdstr = malloc((256 + ft_strlen(tests[i].in))*(sizeof(char)));
			sprintf(cmdstr, "shell_scripts/shell_split.sh %s", tests[i].in);
			FILE *shell = popen(cmdstr, "r");
			tests[i].shellout = NULL;
			size_t length = 0;
			getline(&tests[i].shellout, &length, shell);
			pclose(shell);
			free (cmdstr);
		}
		printf("%11s %s\n", "expected :", tests[i].shellout);

		printf("%11s %s\n\n", "Status :", test_result_str(tests[i].splt2, tests[i].shellout,0));
	}

	// Cleanup
	for (int i=0; tests[i].in; ++i)
	{
		if (tests[i].splt)
		{
			char **splt = tests[i].splt;
			while (*splt)
				free (*(splt++));
			free(tests[i].splt);
		}
		if (tests[i].splt2)
			free(tests[i].splt2);
		if (tests[i].native && tests[i].shellout)
			free(tests[i].shellout);
	}

	return (1);
}

int	main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	setbuf(stdout, NULL);
	test_split();
	return (EXIT_SUCCESS);
}