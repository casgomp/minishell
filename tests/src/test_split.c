/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_split.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 11:35:05 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/28 20:05:30 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ms_rmquotes_expand.h"
#include "tests.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct data {
  t_qes qes;
} t_data;

int process_tstcs(t_tstcs *test_case, void *data) {
  t_data *ptdata = (t_data *)data;
  char **splt = rh_split_quotes(test_case->in, ptdata->qes);
  int stridx = 0;

  if (!splt) {
    stridx += sprintf(&(test_case->info[stridx]), "Error: split = %p | ", splt);
    sprintf(test_case->shellcmd, "echo -n Error");
    return (0);
  }
  stridx += sprintf(&(test_case->info[stridx]), "esc = '%c'",
                    ptdata->qes.esc ? ptdata->qes.esc : '0');
  test_case->minishout = malloc((256 + ft_strlen(test_case->in)) * (sizeof(char)));
  sprintf(test_case->minishout, "°");
  int idx = 0;
  while (splt[idx])
    sprintf(test_case->minishout + strlen(test_case->minishout), "%s°", splt[idx++]);
  if (test_case->shellout == NULL)
    test_case->minishout =
        rh_rmqed(test_case->minishout,
                 (t_qed){.dict = NULL, .esc = '\\', .expnd = '\0', .q = ptdata->qes.q});
  sprintf(test_case->shellcmd, "shell_scripts/shell_split.sh %s", test_case->in);
  return (1);
}

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

  t_qes qes = {.blank = " \t", .q = "\"\'", .esc = '\\', .metachr = "|<>"};
  t_data tdata = (t_data){.qes = qes};

  char test_suite[128];
  sprintf(test_suite, "Test suite: %s", __func__);

  generate_testoutput(test_suite, tests, &process_tstcs, &tdata);

  return (1);
}

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;
  setbuf(stdout, NULL);
  test_split();
  return (EXIT_SUCCESS);
}