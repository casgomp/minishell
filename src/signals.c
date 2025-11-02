/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 17:20:45 by pecastro          #+#    #+#             */
/*   Updated: 2025/11/02 19:39:08 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# define _POSIX_C_SOURCE 200809L
# include "minishell.h"

# include <readline/readline.h>
//# include <readline/history.h>
# include <signal.h>
# include <stdio.h>
# include <unistd.h>

void	sigint_handler(int sig)
{
	g_signal_received = sig;
	write (1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

int	setup_signal(int signum, void(*handler)(int))
{
	struct sigaction sa;

	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask);
	//sigfillset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(signum, &sa, NULL) == -1)
		return (perror("sigaction"), 0);
	return (1);
}

