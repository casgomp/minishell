/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_split.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 11:35:05 by rhaas             #+#    #+#             */
/*   Updated: 2025/10/23 12:59:26 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "msh_rmquotes_expand.h"
#include "tests.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct data {
  t_sqed sqed;
  char *shellScriptPath;
} t_data;

int process_tstcs(t_tstcs *test_case, void *data) {
  t_data *ptdata = (t_data *)data;
  t_qes const qes = rh_sqed2qes(&(ptdata->sqed));
  char **splt = rh_split_quotes(test_case->in, qes);
  int stridx = 0;

  if (!splt) {
    stridx += sprintf(&(test_case->info[stridx]), "Error: split = %p | ", splt);
    sprintf(test_case->shellcmd, "echo -n Error");
    return (0);
  }
  stridx += sprintf(&(test_case->info[stridx]), "esc = '%c'",
                    qes.esc ? qes.esc : '0');
  test_case->minishout = malloc((256 + ft_strlen(test_case->in)) * (sizeof(char)));
  sprintf(test_case->minishout, "°");
  int idx = 0;
  while (splt[idx])
    sprintf(test_case->minishout + strlen(test_case->minishout), "%s°", splt[idx++]);
  if (test_case->shellout == NULL)
  {
	t_qed qed = (t_qed){.q = "\"\'",
                                                                  .rmqe = true,
                                                                  .esc = '\\',
                                                                  .expnd = '\0',
                                                                  .expstat = false,
                                                                  .dict = NULL,
                                                                  .dict_data = NULL,
                                                                  .validchar = NULL,
                                                                  .validchar_data = NULL};
    test_case->minishout = rh_rmqed(test_case->minishout, &qed);
  }
  sprintf(test_case->shellcmd, "%s %s", ptdata->shellScriptPath, test_case->in);
  return (1);
}

int test_split(char *shellScriptPath) {
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
      /* 14 */ {.in = "a > b a> b a >b a>b", .shellout = "°a°>°b°a°>°b°a°>°b°a°>°b°"},
      /* 15 */ {.in = "42 > b 42> b 42 >b 42>b", .shellout = "°42°>°b°42>°b°42°>°b°42>°b°"},
      /* 16 */
      {.in = "42 << b 42<< b 42 <<b 42<<b", .shellout = "°42°<<°b°42<<°b°42°<<°b°42<<°b°"},
      /* 17 */ {.in = "42 | b 42| b 42 |b 42|b", .shellout = "°42°|°b°42°|°b°42°|°b°42°|°b°"},
      /* 18 */
      {.in = "42 || b 42|| b 42 ||b 42||b", .shellout = "°42°||°b°42°||°b°42°||°b°42°||°b°"},
      /*eof */ {.in = NULL},
  };

  // Full list of bash metachar: "|&;()<> \t\n", overlaps with .blank!
  bool firstchar;
  t_sqed sqed = (t_sqed){.q = "\"\'",
                         .rmqe = true,
                         .esc = '\\',
                         .expnd = '$',
                         .blank = " \t",
                         .ifs = " \t\n",
                         .metachr = "|<>",
                         .dict = msh_dict,
                         .dict_data = NULL,
                         .validchar = msh_isvalidvarchar,
                         .validchar_data = &firstchar};
  t_data tdata = (t_data){.sqed = sqed, .shellScriptPath = shellScriptPath};

  char test_suite[128];
  sprintf(test_suite, "Test suite: %s", __func__);

  generate_testoutput(test_suite, tests + 15, &process_tstcs, &tdata);

  return (1);
}

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;
  setbuf(stdout, NULL);
  char *shellScriptPath = argv[0];
  *(strrchr(argv[0], '/') + 1) = '\0';
  shellScriptPath = ft_strjoin(shellScriptPath, "../shell_scripts/shell_split.sh");
  test_split(shellScriptPath);
  free(shellScriptPath);
  return (EXIT_SUCCESS);
}