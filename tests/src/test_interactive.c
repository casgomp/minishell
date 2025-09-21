/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_interactive.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 11:35:05 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/21 13:33:39 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tests.h"
#include "libft.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int test_interactive()
{
	int a = isatty(0);
	char *n = ttyname(0);
	ft_printf("Stdin fd 0 is %sa tty: %s\n", a?"":"not ", n);
	return (1);
}

int	main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	setbuf(stdout, NULL);
	test_interactive();
	return (EXIT_SUCCESS);
}