/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 11:35:05 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/21 20:51:38 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ms_rmquotes_expand.h"
#include "libft.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <fcntl.h>

int process_cmdline(char const *const in)
{
	int idx;
	char newline;
	char **const split = ft_split_quotes(in,
		(t_qes){.splt = ' ', .specialsplt="", .q="\"\'", .esc='\\'});

	newline = '\n';
	if (split && *split)
	{
		idx = 0;
		if (!ft_strncmp(split[idx], "echo", 5))
		{
			while (split[++idx] && !(ft_strncmp(split[idx], "-n", 3)))
				newline = '\b';
			--idx;
			while (split[++idx])
				ft_printf("%s ", split[idx]);
			ft_printf("%c", newline);
		}
	}
	return (1);
}

int	main(int argc, char *argv[])
{
	char *in;
	char *prompt;
	int	fdscript;
	bool interactive;

	fdscript = -1;
	if (argc == 2)
	{
		fdscript = open(argv[1], O_RDONLY);
		if (fdscript == -1)
			return (perror(argv[1]), 127);
		dup2(fdscript, 0);
	}

	interactive = isatty(0);
	while (true)
	{
		if (interactive)
		{
			prompt = ms_rmquotes_expand("$USER@minishell$ ",
					(t_qed){.dict=getenv, .esc='\'', .expnd='$', .q="\"\'"});
			in = readline(prompt);
			free (prompt);
		}
		else
		{
			in = get_next_line(0);
			if (in && in[0] && in[ft_strlen(in)-1] == '\n')
				in[ft_strlen(in)-1] = '\0';
		}
		if (in)
			add_history(in);
		if (!in || !ft_strncmp(in, "exit", 5))
		{
			if (interactive)
				printf("exit\n");
			free(in);
			break;
		}
		process_cmdline(in);
		free(in);
	}
	rl_clear_history();
	clear_history();
	if (fdscript != -1)
		close(fdscript);
	return (EXIT_SUCCESS);
}
