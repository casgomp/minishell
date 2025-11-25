/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 17:20:45 by pecastro          #+#    #+#             */
/*   Updated: 2025/11/25 14:50:45 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <readline/readline.h>
#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

void	sigint_handler_heredoc(int sig)
{
	g_signal_received = sig;
	close(STDIN_FILENO);
	write (1, "^C", 2);
}

int	setup_signal_heredoc(t_sa *psa_old, void (*handler)(int))
{
	if (sigaction(SIGINT, NULL, &(psa_old->sa_int)) == -1)
		return (0);
	if (sigaction(SIGQUIT, NULL, &(psa_old->sa_quit)) == -1)
		return (0);
	msh_setup_signal(SIGINT, handler);
	msh_setup_signal(SIGQUIT, SIG_IGN);
	return (1);
}

void	reset_handlers_heredoc(struct sigaction *sa_old_int,
	struct sigaction *sa_old_quit)
{
	sigaction(SIGINT, sa_old_int, NULL);
	sigaction(SIGQUIT, sa_old_quit, NULL);
}
