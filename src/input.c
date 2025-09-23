/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 11:35:05 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/23 16:46:56 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <fcntl.h>

int process_cmdline(t_cd *pcd, char const *const in)
{
	char **const split = ft_split_quotes(in, pcd->default_qes);

	if (split && *split)
	{
		if (ms_builtins(pcd, split))
			return (1);
	}
	return (0);
}

int	setup_stdin(t_cd *pcd, int const argc, char **const argv)
{
	pcd->fdscript = -1;
	if (argc == 2)
	{
		pcd->fdscript = open(argv[1], O_RDONLY);
		if (pcd->fdscript == -1)
		{
			perror(argv[1]);
			exit (127);
		}
		else if (dup2(pcd->fdscript, 0) == -1)
		{
			perror(argv[1]);
			exit (9);
		}
	}
	else if (argc > 2)
	{
		ft_printf("usage: minishell [scriptfile]\n");
		exit (7);
	}
	pcd->interactive = isatty(0);
	return (1);
}

int	get_cmdline_add_history(t_cd *pcd, char **pcmdline)
{
	char		*prompt;
	size_t		cmdln_len;

	if (!pcmdline)
		return (0);
	if (pcd->interactive)
	{
		prompt = ms_rmqts_esc_xpnd("$USER@minishell$ ", pcd->default_qed);
		if (prompt)
		{
			*pcmdline = readline(prompt);
			free (prompt);
			if (*pcmdline)
				add_history(*pcmdline);
		}
	}
	else
	{
		*pcmdline = get_next_line(0);
		if (*pcmdline)
		{
			cmdln_len = ft_strlen(*pcmdline);
			if (cmdln_len > 0 && (*pcmdline)[cmdln_len - 1] == '\n')
				(*pcmdline)[cmdln_len - 1] = '\0';
		}
	}
	if (!*pcmdline)
	{
		pcd->exit = true;
		return (0);
	}
	return (1);
}
