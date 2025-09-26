/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_quote_expand.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 11:35:05 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/26 13:42:34 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ms_rmquotes_expand.h"
#include "tests.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct data
{
	t_qed qed;
} t_data;

int process_tstcs(t_tstcs *test_case, void *data)
{
	t_data *ptdata = (t_data*)data;

	sprintf(test_case->info, "esc = '%c'", ptdata->qed.esc ? ptdata->qed.esc : '0');
	test_case->minishout = rh_rmqed(test_case->in, ptdata->qed);
	sprintf(test_case->shellcmd, "echo -n %s", test_case->in);
	return (1);
}

int test_quote_expand() {
  t_tstcs tests[] = {
      /*  0 */ {.in = "\"a\\\"x\\\"b\"", .shellout = NULL},
    //   /*  1 */ {.in = "\"ax\\\"b\"", .shellout = NULL},
    //   /*  2 */ {.in = "\\\'a\\\"x\\\"b\\\'", .shellout = NULL},
    //   /*  3 */ {.in = "a-$SHELL-b", .shellout = NULL},
    //   /*  4 */ {.in = "a-\\$SHELL-b", .shellout = NULL},
    //   /*  5 */ {.in = "a-'$SHELL'-b", .shellout = NULL},
    //   /*  6 */ {.in = "a-\\'$SHELL\\'-b", .shellout = NULL},
    //   /*  7 */ {.in = "\"a-'$SHELL'-b\"", .shellout = NULL},
    //   /*  8 */ {.in = "a-$SHED3-b", .shellout = NULL},
    //   /*  9 */ {.in = "a-$SHELL-b-'$SHELL'-c-", .shellout = NULL},
    //   /* 10 */ {.in = "\"a-$SHELL-b-'$SHELL'-c-\"", .shellout = NULL},
    //   /* 11 */ {.in = "\"a-$3SHELL-b-'$SHELL'-c-\"", .shellout = NULL},
    //   /* 12 */ {.in = "\"a-$<-b\"", .shellout = NULL},
    //   /* 13 */ {.in = "\"a-$A<-b\"", .shellout = NULL},
      /*eof */ {.in = NULL},
  };

  bool firstchar;
  t_qed qed = (t_qed){.q = "\"\'",
                      .esc = '\\',
                      .expnd = '$',
                      .dict = ms_dict,
                      .dict_data = NULL,
                      .validchar = ms_isvalidvarchar,
                      .validchar_data = &firstchar};

	t_data tdata = (t_data){.qed = qed};
	generate_testoutput(tests, qed.esc, &process_tstcs, &tdata);
//   printf("Test suite: %s\n", __func__);
//   for (int i = 0; tests[i].in; ++i) {
//     tests[i].native = (tests[i].shellout == NULL);

//     printf("Test %2d   : (shell native = %s)\n", i, tests[i].native ? "true" : "false");
//     printf("%11s %s\n", "input :", tests[i].in);

//     tests[i].minishout = rh_rmqed(tests[i].in, qed);
//     if (tests[i].native) {
//       tests[i].minishout = tests[i].minishout;
	  
//       char cmdstr[128];
//       sprintf(cmdstr, "echo -n %s", tests[i].in);
//       FILE *shell = popen(cmdstr, "r");
//       size_t length = 0;
//       getline(&tests[i].shellout, &length, shell);
//       pclose(shell);
//     }
//     printf("esc = '%c' : %s\n", qed.esc ? qed.esc : '0', tests[i].minishout);
//     printf("%11s %s\n", "expected :", tests[i].shellout);
//     printf("%11s %s\n\n", "Status :", test_result_str(tests[i].minishout, tests[i].shellout, qed.esc == '\\'));
//   }

//   // Cleanup
//   for (int i = 0; tests[i].in; ++i) {
//     if (tests[i].minishout)
//       free(tests[i].minishout);
//     if (tests[i].native && tests[i].shellout)
//       free(tests[i].shellout);
//   }

  return (1);
}

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;
  setbuf(stdout, NULL);
  test_quote_expand();
  return (EXIT_SUCCESS);
}
