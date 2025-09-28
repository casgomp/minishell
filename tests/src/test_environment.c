/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_environment.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 11:35:05 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/26 11:50:27 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "tests.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int test_environment() {
  char *envval = getenv("SHELL");
  (void)envval;
  envval = getenv("DOES_NOT_EXIST"); // returns nullptr because var does not exist
  envval = getenv(
      "42"); // returns nullptr (because there never are any envvars starting with a digit)
  envval = getenv(""); // returns nullptr
  // envval = getenv(NULL); // segfault
  return (1);
}

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;
  setbuf(stdout, NULL);
  test_environment();
  return (EXIT_SUCCESS);
}