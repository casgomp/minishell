/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_split.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 11:35:05 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/26 13:41:05 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ms_rmquotes_expand.h"
#include "tests.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct testcase {
  char *in;
  char **splt;
  char *minishout;
  char *shellout;
  bool native;
} t_tstcs;

int test_split() {
  t_tstcs tests[] = {
      /*  0 */ {.in = "ein wort", .shellout = NULL},
      /*  1 */ {.in = "ein|wort", .shellout = "°ein°|°wort°"},
      /*  2 */ {.in = "ein||wort", .shellout = "°ein°||°wort°"},
      /*  3 */ {.in = "ein ||wort", .shellout = "°ein°||°wort°"},
      /*  4 */ {.in = "ein |wort", .shellout = "°ein°|°wort°"},
      /*  5 */ {.in = "ein |   wort", .shellout = "°ein°|°wort°"},
      /*  6 */ {.in = "ein >>   wort", .shellout = "°ein°>>°wort°"},
      /*  7 */ {.in = "ein | | wort", .shellout = "°ein°|°|°wort°"},
      /*  8 */ {.in = "'ein wort' ohne \"hh\"", .shellout = "°'ein wort'°ohne°\"hh\"°"},
      /*  9 */ {.in = "\'ein wort\' ohne \"hh\"", .shellout = NULL},
      /* 10 */ {.in = "\"a\\\"b \\\"\" c", .shellout = NULL},
      /* 11 */ {.in = "\"ein \\\"wort\\\"\" ohne \"hh\"", .shellout = NULL},
      /* 12 */ {.in = "'ein \"wort\"' ohne \"h h\"", .shellout = NULL},
      /* 13 */ {.in = "'ein \"wort\"' ohne 'h h'", .shellout = NULL},
      /*eof */ {.in = NULL},
  };

  t_qes qes = {.splt = " \t", .q = "\"\'", .esc = '\\', .specialsplt = "|<>"};

  printf("Test suite: %s\n", __func__);
  for (int i = 0; tests[i].in; ++i) {
    tests[i].native = (tests[i].shellout == NULL);

    printf("Test %2d   : (shell native = %s)\n", i, tests[i].native ? "true" : "false");
    printf("%11s %s\n", "input :", tests[i].in);

    tests[i].splt = rh_split_quotes(tests[i].in, qes);
    if (!tests[i].splt) {
      printf("%p\n", tests[i].splt);
      continue;
    }
    tests[i].minishout = malloc((256 + ft_strlen(tests[i].in)) * (sizeof(char)));
    sprintf(tests[i].minishout, "°");
    int idx = 0;
    while (tests[i].splt[idx])
      sprintf(tests[i].minishout + strlen(tests[i].minishout), "%s°", tests[i].splt[idx++]);
    if (tests[i].native) {
      tests[i].minishout = rh_rmqed(tests[i].minishout, (t_qed){.dict = NULL, .esc = '\\', .expnd = '\0', .q = qes.q});

      char cmdstr[128];
      sprintf(cmdstr, "shell_scripts/shell_split.sh %s", tests[i].in);
      FILE *shell = popen(cmdstr, "r");
      size_t length = 0;
      getline(&tests[i].shellout, &length, shell);
      pclose(shell);
    }
    printf("esc = '%c' : %s\n", qes.esc ? qes.esc : '0', tests[i].minishout);
    printf("%11s %s\n", "expected :", tests[i].shellout);
    printf("%11s %s\n\n", "Status :", test_result_str(tests[i].minishout, tests[i].shellout, qes.esc == '\\'));
  }

  // Cleanup
  for (int i = 0; tests[i].in; ++i) {
    if (tests[i].splt) {
      char **splt = tests[i].splt;
      while (*splt)
        free(*(splt++));
      free(tests[i].splt);
    }
    if (tests[i].minishout)
      free(tests[i].minishout);
    if (tests[i].native && tests[i].shellout)
      free(tests[i].shellout);
  }

  return (1);
}

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;
  setbuf(stdout, NULL);
  test_split();
  return (EXIT_SUCCESS);
}