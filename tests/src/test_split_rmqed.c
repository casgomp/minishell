/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_split_rmqed.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 11:35:05 by rhaas             #+#    #+#             */
/*   Updated: 2025/10/27 15:16:33 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "msh_compile_cmds.h"
#include "tests.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct data {
  t_sqed sqed;
} t_data;

int process_tstcs(t_tstcs *test_case, void *data) {
  t_data *ptdata = (t_data *)data;
  char **splt = rh_split_rmqed(test_case->in, ptdata->sqed);
  int stridx = 0;

  if (!splt) {
    stridx += sprintf(&(test_case->info[stridx]), "Error: split = %p | ", splt);
    sprintf(test_case->shellcmd, "echo -n Error");
    return (0);
  }
  stridx += sprintf(&(test_case->info[stridx]), "esc = '%c'",
                    ptdata->sqed.esc ? ptdata->sqed.esc : '0');
  test_case->minishout = malloc((256 + ft_strlen(test_case->in)) * (sizeof(char)));
  sprintf(test_case->minishout, "°");
  int idx = 0;
  while (splt[idx])
    sprintf(test_case->minishout + strlen(test_case->minishout), "%s°", splt[idx++]);
  if (test_case->shellout == NULL)
  {
	t_qed qed = rh_sqed2qed(&(ptdata->sqed));
    test_case->minishout = rh_rmqed(test_case->minishout, &qed);
  }
  sprintf(test_case->info, "esc = '%c'", ptdata->sqed.esc ? ptdata->sqed.esc : '0');
  sprintf(test_case->shellcmd, "echo -n %s", test_case->in);
  return (1);
}

int test_split_rmqed() {
  t_tstcs tests[] = {
      /*  0 */ {.in = "f1 >out <in \"<lit\" a'<'b", .shellout = NULL},
      /*  0 */ {.in = "f1 2>&1 <in \"<lit\" a'<'b", .shellout = NULL},
      /*  1 */ {.in = "ein|wort", .shellout = "°ein°|°wort°"},
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
  t_data tdata = (t_data){.sqed = sqed};

  char test_suite[128];
  sprintf(test_suite, "Test suite: %s", __func__);

  generate_testoutput(test_suite, tests, &process_tstcs, &tdata);

  return (1);
}

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;
  setbuf(stdout, NULL);
  test_split_rmqed();
  return (EXIT_SUCCESS);
}