/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_quote_expand.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 11:35:05 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/26 10:08:22 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ms_rmquotes_expand.h"
#include "tests.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct testcase {
  char *in;
  char *out;
  char *shellout;
} t_tstcs;

int test_quote_expand() {
  t_tstcs tests[] = {
      /*  0 */ {.in = "\"a\\\"x\\\"b\"", .shellout = NULL},
      /*  1 */ {.in = "\"ax\\\"b\"", .shellout = NULL},
      /*  2 */ {.in = "\\\'a\\\"x\\\"b\\\'", .shellout = NULL},
      /*  3 */ {.in = "a-$SHELL-b", .shellout = NULL},
      /*  4 */ {.in = "a-\\$SHELL-b", .shellout = NULL},
      /*  5 */ {.in = "a-'$SHELL'-b", .shellout = NULL},
      /*  6 */ {.in = "a-\\'$SHELL\\'-b", .shellout = NULL},
      /*  7 */ {.in = "\"a-'$SHELL'-b\"", .shellout = NULL},
      /*  8 */ {.in = "a-$SHED3-b", .shellout = NULL},
      /*  9 */ {.in = "a-$SHELL-b-'$SHELL'-c-", .shellout = NULL},
      /* 10 */ {.in = "\"a-$SHELL-b-'$SHELL'-c-\"", .shellout = NULL},
      /* 11 */ {.in = "\"a-$3SHELL-b-'$SHELL'-c-\"", .shellout = NULL},
      /* 12 */ {.in = "\"a-$<-b\"", .shellout = NULL},
      /* 13 */ {.in = "\"a-$A<-b\"", .shellout = NULL},
      /*eof */ {.in = NULL},
  };

  for (int i = 0; tests[i].in; ++i) {
    printf("Test %2d: %s\n", i, tests[i].in);

    bool firstchar;
    tests[i].out = rh_rmqed(tests[i].in, (t_qed){.q = "\"\'",
                                                 .esc = '\\',
                                                 .expnd = '$',
                                                 .dict = ms_dict,
                                                 .dict_data = NULL,
                                                 .validchar = ms_isvalidvarchar,
                                                 .validchar_data = &firstchar});
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
  for (int i = 0; tests[i].in; ++i) {
    if (tests[i].out)
      free(tests[i].out);
    if (tests[i].shellout)
      free(tests[i].shellout);
  }

  return (1);
}

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;
  test_quote_expand();
  return (EXIT_SUCCESS);
}
