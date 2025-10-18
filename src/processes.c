/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 13:50:29 by pecastro          #+#    #+#             */
/*   Updated: 2025/10/18 19:37:17 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "msh_rmquotes_expand.h"
#include "libft.h"

#include <stdlib.h>
#include <unistd.h>

typedef struct	s_fdcls
{
	int	pre1[2];
	int	pre2[2];
}	t_fdcl;

int	count_children(t_cmd *curcmd)
{
	int	nchild;

	nchild = -1;
	while (curcmd)
	{
		if (curcmd->ctrlchr == '|' || curcmd->iolst)
			nchild ++;
	}
	return (nchild);
}

int	parent_setup(t_cmd *curcmd, int *pids, int i)
{
	static t_fdcl	fdcl = {{-1,-1}, {-1, -1}};
	int				fdpipe[2];

	//if heredoc, all inputs must be stored in a char *buffer[N]
	if (fdcl.pre2[0] != -1)
		close(fdcl.pre2[0]);
	if (fdcl.pre1[1] != -1)
		close(fdcl.pre1[1]);
	if (curcmd->ctrlchr == '|')
	{
		if (pipe(fdpipe) == -1);
			return (perror("Error: pipe()"), 1);
	}
	if (curcmd->next)
	{
		pids[i] = fork();
		if (pids[i] == -1)
		return (perror("Error: pipe()"), 1);
	}	
	//reassign fdcls for next cmd:
	fdcl.pre2[0] = fdcl.pre1[0];
	fdcl.pre2[1] = fdcl.pre1[1];
	fdcl.pre1[0] = fdpipe[0];
	fdcl.pre1[1] = fdpipe[1];
	if (pids[i] == 0)
	{
		//function for child: pipe closing, redirections and executions;
	}
	//return (0); check return values bcs i'm using 1 for error and 0 for success.
}
