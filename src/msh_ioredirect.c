/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_ioredirect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:12:30 by rhaas             #+#    #+#             */
/*   Updated: 2025/11/14 16:59:25 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "msh_builtins.h"
#include "msh_compile_cmds.h"
#include "msh_compile_cmds_intern.h"

#include <limits.h>
#include <stdlib.h>

static bool	msh_ioredirect_from_fd(t_tca *ptca, const char *const token,
	char const **const piooperator, t_llong *pfd_new)
{
	int		success;
	int 	ioopstartpos;

	ioopstartpos = ft_strlen(token) - 1;
	while (ioopstartpos > 0 && ft_strchr("<>", token[ioopstartpos - 1]))
		--ioopstartpos;
	*pfd_new = ft_atoll_check_input(token, INT_MAX, &success);
	if (*pfd_new < 0 || success <= 0 || success != ioopstartpos)
	{
		*pfd_new = -1;
		if (ioopstartpos > 0)
		{
			if (!get_curarg(ptca))
				return (false);
			ptca->curarg->arg = ft_substr(token, 0, ioopstartpos);
		}
	}
	*piooperator = &(token[ioopstartpos]);
	return (true);
}

static void	msh_ioredirect_settype(t_tca *ptca,
	char const *const iooperator, t_llong fd_new)
{
	if (!ft_strcmp(iooperator, ">"))
		ptca->curcmd->curio->tredirect = (t_rd)fd2file_trunc;
	else if (!ft_strcmp(iooperator, ">>"))
		ptca->curcmd->curio->tredirect = (t_rd)fd2file_append;
	else if (!ft_strcmp(iooperator, "<"))
		ptca->curcmd->curio->tredirect = (t_rd)file2fd;
	else if (!ft_strcmp(iooperator, "<<"))
		ptca->curcmd->curio->tredirect = (t_rd)heredoc2fd;
	if (fd_new == -1)
	{
		if ((ptca->curcmd->curio->tredirect == (t_rd)fd2file_trunc)
			|| (ptca->curcmd->curio->tredirect == (t_rd)fd2file_append))
			fd_new = 1;
		if ((ptca->curcmd->curio->tredirect == (t_rd)file2fd)
			|| (ptca->curcmd->curio->tredirect == (t_rd)heredoc2fd))
			fd_new = 0;
	}
	ptca->curcmd->curio->new_fd = fd_new;
}

void	msh_syntaxerror(t_cd *pcd,
	char const *const token, char const *const cmdline)
{
	char	buf[1024];

	if (token)
	{
		ft_sprintf(buf, "syntax error near unexpected token `%s'", token);
		msh_error((char const *[]){buf, }, 1, -1);
	}
	if (!pcd->interactive && cmdline)
	{
		if (token)
			ft_sprintf(buf, "`%s'", cmdline);
		else
			ft_sprintf(buf, "syntax error: %s", cmdline);
		msh_error((char const *[]){buf, }, 1, -1);		
	}
	pcd->exit_status = 2;
}

static bool	msh_ioredirect_setiotarget(t_cd *pcd, t_tca *ptca,
	t_sqed const sqed, char const *const iotarget)
{
	char **const	token_ws = msh_expand_wordsplit_token(iotarget, &sqed);

	if (iotarget && ((ft_strlen(iotarget) == 0)
			|| !ft_strchr(sqed.metachr, iotarget[0])))
	{
		if (!token_ws || token_ws[0] == (char *) NULL
			|| token_ws[1] != (char *) NULL)
		{
			msh_error((char const *[]){iotarget, "ambiguous redirect"}, 2, -1);
			pcd->exit_status = 130;
			return (false);
		}
		ptca->curcmd->curio->fname_delim = ft_strdup(token_ws[0]);
		return (rh_free_simple_array((void ***)&token_ws), true);
	}
	msh_syntaxerror(pcd, iotarget, ptca->tosplit);
	// ft_sprintf(buf, "syntax error near unexpected token `%s'", iotarget);
	// msh_error((char const *[]){buf, }, 1, -1);
	// if (pcd->interactive)
	// {
	// 	ft_sprintf(buf, "`%s'", ptca->tosplit);
	// 	msh_error((char const *[]){buf, }, 1, -1);
	// }
	// pcd->exit_status = 2;
	return (false);
}

bool	msh_process_ioredirector(t_cd *pcd, t_tca *ptca,
	int const	tokenidx, t_sqed const sqed)
{
	char const *const	token = ptca->tokens[tokenidx];
	char const *const	iotarget = ptca->tokens[tokenidx + 1];
	char const			*iooperator;
	t_llong				fd_new;
	char				buf[256];

	if (!get_iolstelem(ptca))
		return (false);
	iooperator = token;
	if (!msh_ioredirect_from_fd(ptca, token, &iooperator, &fd_new))
		return (false);
	if (fd_new > 1023)
	{
		ft_sprintf(buf,"%d", fd_new);
		pcd->exit_status = 1;
		msh_error((char const *[]){buf, "Bad file descriptor"}, 2, -1);
		return (false);
	}
	msh_ioredirect_settype(ptca, iooperator, fd_new);
	if (!msh_ioredirect_setiotarget(pcd, ptca, sqed, iotarget))
		return (false);
	if ((ptca->curcmd->curio->tredirect == (t_rd)rd_default)
		|| (!ptca->curcmd->curio->fname_delim))
	{
		pcd->exit_status = 130;
		msh_error((char const *[]){"Internal Error", }, 1, -1);
	}
	return (true);
}
