/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_quote_expand.c                                :+:      :+:    :+:   */
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

#include <stdio.h>
#include <stdlib.h>

typedef struct data {
  t_qed qed;
} t_data;

int process_tstcs(t_tstcs *test_case, void *data) {
  t_data *ptdata = (t_data *)data;

  sprintf(test_case->info, "esc = '%c'", ptdata->qed.esc ? ptdata->qed.esc : '0');
  test_case->minishout = rh_rmqed(test_case->in, &(ptdata->qed));
  sprintf(test_case->shellcmd, "echo -n %s", test_case->in);
  return (1);
}

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
      /* 14 */ {.in = "\"'''$USER\"''\\'", .shellout = NULL},
      /*eof */ {.in = NULL},
  };

  bool firstchar;
  t_qed qed = (t_qed){.q = "\"\'",
                      .rmqe = true,
                      .esc = '\\',
                      .expnd = '$',
                      .expstat = false,
                      .dict = msh_dict,
                      .dict_data = NULL,
                      .validchar = msh_isvalidvarchar,
                      .validchar_data = &firstchar};
  t_data tdata = (t_data){.qed = qed};

  char test_suite[128];
  sprintf(test_suite, "Test suite: %s", __func__);

  generate_testoutput(test_suite, tests, &process_tstcs, &tdata);

  return (1);
}

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;
  setbuf(stdout, NULL);
  test_quote_expand();
  return (EXIT_SUCCESS);
}
