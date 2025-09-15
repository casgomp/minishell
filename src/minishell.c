/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 11:35:05 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/15 13:27:21 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

int	main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	char *in;
	while ((in = readline("<fancy prompt>$ ")))
	{
		if (in && *in != ' ')
			add_history(in);
		if (in && !ft_strncmp(in, "exit", 5))
		{
			free(in);
			break;
		}
		printf("Test io: %s\n", in);
		free(in);
	}
	rl_clear_history();
	clear_history();
	return (EXIT_SUCCESS);
}