/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 17:20:45 by pecastro          #+#    #+#             */
/*   Updated: 2025/11/05 17:36:12 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# define _POSIX_C_SOURCE 200809L
# include "minishell.h"

# include <readline/readline.h>
//# include <readline/history.h>
# include <signal.h>
# include <stdio.h>
# include <unistd.h>

void	reset_handlers_heredoc(struct sigaction *sa)
{
	sa->sa_handler = SIG_DFL;
	sigemptyset(&sa->sa_mask);
	//sa->sa_flags = 0;
	sigaction(SIGINT, sa, NULL);
	sigaction(SIGQUIT, sa, NULL);
}

void	sigint_handler_heredoc(int sig)
{
	g_signal_received = sig;
	if (sig == SIGINT)
		write(1, "^C", 2);
	write(1, "\n", 1);
	//rl_on_new_line();
	//rl_replace_line("", 0);
	close(STDIN_FILENO);
}

void	sigint_handler(int sig)
{
	g_signal_received = sig;
	write (1, "\n", 1);
	if (sig == SIGINT)
		write (1, "^C\n", 3);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

int	setup_signal_heredoc(struct sigaction *sa, void(*handler)(int))
{
	ft_memset(sa, 0, sizeof(*sa));
	sa->sa_handler = handler;
	sigemptyset(&sa->sa_mask);
	sa->sa_flags = 0;
	if (sigaction(SIGINT, sa, NULL) == -1)
		return (perror("sigaction"), 0);
	sa->sa_handler = SIG_IGN;
	sigaction(SIGQUIT, sa, NULL);//redundant? already ignored in parent.
	return (1);
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