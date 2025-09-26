/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 15:21:09 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/17 18:00:31 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TESTS_H
#define TESTS_H

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

#endif // TESTS_H