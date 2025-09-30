/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_environment.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 11:35:05 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/30 18:11:13 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "tests.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern char **environ;

size_t env_size()
{
  char **envsave = environ;
  size_t envsize = 0;

  while (*(envsave++))
  	++envsize;
  return (envsize);
}

int test_environment() {
  char *envval = getenv("SHELL");
  (void)envval;
  envval = getenv("DOES_NOT_EXIST"); // returns nullptr because var does not exist
  envval = getenv(
      "42"); // returns nullptr (because there never are any envvars starting with a digit)
  envval = getenv(""); // returns nullptr
  // envval = getenv(NULL); // segfault

  char **envsave = environ;
  size_t envsize = env_size();

  setenv("VAR", "ValueWert", 0);
  envval = getenv("VAR");
  printf("envptrsave: %p\n", envsave);
  printf("envptr    : %p\n", environ);

  envsize = env_size();  
 
  unsetenv("VAR");
  envval = getenv("VAR");
  printf("envptrsave: %p\n", envsave);
  printf("envptr    : %p\n", environ);
 
  envsize = env_size();
   
  setenv("VAR", "ValueWert", 0);
  envval = getenv("VAR");  
  printf("envptrsave: %p\n", envsave);
  printf("envptr    : %p\n", environ);

  envsize = env_size();
   
  setenv("VAR2", "ValueWert", 0);
  envval = getenv("VAR2");  
  printf("envptrsave: %p\n", envsave);
  printf("envptr    : %p\n", environ);

  envsize = env_size();

  (void)envsize;
  return (1);
}

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;
  setbuf(stdout, NULL);
  test_environment();
  return (EXIT_SUCCESS);
}