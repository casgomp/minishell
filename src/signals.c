/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 17:20:45 by pecastro          #+#    #+#             */
/*   Updated: 2025/11/25 14:50:15 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <readline/readline.h>
#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

void	sigint_hndlr_wait(t_cd *pcd, int wstatus, bool last_cmd, int *sgntcnt)
{
	pcd->exit_status = 128 + WTERMSIG(wstatus);
	if (WTERMSIG(wstatus) == SIGINT && !*sgntcnt)
	{
		*sgntcnt = 1;
		write(1, "\n", 1);
	}
	else if (WTERMSIG(wstatus) == SIGQUIT && WCOREDUMP(wstatus) && last_cmd)
		write(1, "Quit (core dumped)\n", 19);
}

void	sigint_handler(int sig)
{
	g_signal_received = sig;
	write (1, "^C\n", 3);
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

int	msh_setup_signal(int signum, void (*handler)(int))
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(signum, &sa, NULL) == -1)
		return (perror("sigaction"), 0);
	return (1);
}
