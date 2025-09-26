/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 15:21:09 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/26 13:42:58 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TESTS_H
#define TESTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *test_result_str(char const *const s1, char const *const s2, int strict) {
  if (!(s1 && s2))
    return ("\x1b[1;37m empty str(s) - undefined result\x1b[0m");
  if (!strcmp(s1, s2))
    return ("\x1b[1;32mOK\x1b[0m");
  else {
    if (strict == 1)
      return ("\x1b[1;31m### KO ###\x1b[0m");
    else
      return ("\x1b[0;33m# ?? #\x1b[0m");
  };
}

typedef struct testcase {
  char *in;
  char *minishout;
  char shellcmd[128];
  char *shellout;
  char info[128];
//   bool native;
} t_tstcs;

bool generate_testoutput(
	t_tstcs tests[], char esc,
	int (*processtestcase)(t_tstcs *testcase, void *data), void *data)
{
	bool native;
	
  printf("Test suite: %s\n", __func__);
  for (int i = 0; tests[i].in; ++i) {
    native = (tests[i].shellout == NULL);

    printf("Test %2d   : (shell native = %s)\n", i, native ? "true" : "false");
    printf("%11s %s\n", "input :", tests[i].in);

    (*processtestcase)(&tests[i], data);
    if (native) {
      FILE *shell = popen(tests[i].shellcmd, "r");
      size_t length = 0;
      getline(&tests[i].shellout, &length, shell);
      pclose(shell);
    }
    printf("%11s %s\n", "info :", tests[i].info);
	printf("%11s %s\n", "msout :", tests[i].minishout);
    printf("%11s %s\n", "bashout :", tests[i].shellout);
    printf("%11s %s\n\n", "Status :",
		test_result_str(tests[i].minishout, tests[i].shellout, esc == '\\'));
  }

  // Cleanup
  for (int i = 0; tests[i].in; ++i) {
    if (tests[i].minishout)
      free(tests[i].minishout);
    if (native && tests[i].shellout)
      free(tests[i].shellout);
  }
  return (true);
}

#endif // TESTS_H