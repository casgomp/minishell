/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 17:20:45 by pecastro          #+#    #+#             */
/*   Updated: 2025/11/24 15:36:50 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <readline/readline.h>
#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

void	reset_handlers_heredoc(struct sigaction *sa_old_int,
	struct sigaction *sa_old_quit)
{
	sigaction(SIGINT, sa_old_int, NULL);
	sigaction(SIGQUIT, sa_old_quit, NULL);
}

void	sigint_handler_wait(t_cd *pcd, int wstatus, int ncmds, int idx, int *sgntcnt)
{
	pcd->exit_status = 128 + WTERMSIG(wstatus);
	if (WTERMSIG(wstatus) == SIGINT && !*sgntcnt)
	{
		*sgntcnt = 1;
		write(1, "\n", 1);
	}
	else if (WTERMSIG(wstatus) == SIGQUIT && WCOREDUMP(wstatus) && idx == ncmds - 1)
		write(1, "Quit (core dumped)\n", 19);
}

void	sigint_handler_heredoc(int sig)
{
	g_signal_received = sig;
	close(STDIN_FILENO);
	write (1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	// rl_redisplay();
}

void	sigint_handler(int sig)
{
	g_signal_received = sig;
	write (1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	setup_signal_wait(struct sigaction *old_sigint)
{
	struct sigaction	sa_ignore;

	ft_memset(&sa_ignore, 0, sizeof(sa_ignore));
	sa_ignore.sa_handler = SIG_IGN;
	sigemptyset(&sa_ignore.sa_mask);
	sa_ignore.sa_flags = 0;
	sigaction(SIGINT, &sa_ignore, old_sigint);
}

int	setup_signal_heredoc(t_sa *psa_old, void (*handler)(int))
{
	if (sigaction(SIGINT, NULL, &(psa_old->sa_int)) == -1)
		return (0); //correct return? perror?
	if (sigaction(SIGQUIT, NULL, &(psa_old->sa_quit)) == -1)
		return (0); //correct return? perror?
	msh_setup_signal(SIGINT, handler);
	msh_setup_signal(SIGQUIT, SIG_IGN);
	return (1);
}

int	msh_setup_signal(int signum, void (*handler)(int))
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	// sa.sa_flags = 0;
	if (sigaction(signum, &sa, NULL) == -1)
		return (perror("sigaction"), 0);
	return (1);
}
