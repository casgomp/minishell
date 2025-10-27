/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_ioredirect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:12:30 by rhaas             #+#    #+#             */
/*   Updated: 2025/10/27 16:14:30 by rhaas            ###   ########.fr       */
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
	int	success;

	*pfd_new = ft_atoll_check_input(token, INT_MAX, &success);
	if (*pfd_new < 0 || success <= 0 || !ft_strchr("<>", token[success]))
	{
		*pfd_new = -1;
		success = ft_strlen(token) - 1;
		while (success > 0 && ft_strchr("<>", token[success - 1]))
			--success;
		if (!get_curarg(ptca))
			return (false);
		if (success > 0)
		{
			ptca->curarg->arg = ft_substr(token, 0, success);
			*piooperator = &(token[success]);
		}
	}
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

static bool	msh_ioredirect_setiotarget(t_cd *pcd, t_tca *ptca,
	t_sqed const sqed, char const *const iotarget)
{
	char **const	token_ws = msh_expand_wordsplit_token(iotarget, &sqed);
	char			buf[1024];

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
		return (rh_free_simple_array((void **)token_ws), true);
	}
	ft_sprintf(buf, "syntax error near unexpected token `%s'", iotarget);
	msh_error((char const *[]){buf, }, 1, -1);
	ft_sprintf(buf, "`%s'", ptca->tosplit);
	msh_error((char const *[]){buf, }, 1, -1);
	pcd->exit_status = 2;
	return (false);
}

bool	msh_process_ioredirector(t_cd *pcd, t_tca *ptca,
	int const	tokenidx, t_sqed const sqed)
{
	char const *const	token = ptca->tokens[tokenidx];
	char const *const	iotarget = ptca->tokens[tokenidx + 1];
	char const			*iooperator;
	t_llong				fd_new;

	if (!get_iolstelem(ptca))
		return (false);
	iooperator = token;
	if (!msh_ioredirect_from_fd(ptca, token, &iooperator, &fd_new))
		return (false);
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
