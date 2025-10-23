/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_rmquotes_expand.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:12:30 by rhaas             #+#    #+#             */
/*   Updated: 2025/10/23 16:15:54 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "msh_builtins.h"
#include "msh_rmquotes_expand.h"
#include "msh_rmquotes_expand_intern.h"

#include <limits.h>
#include <stdlib.h>

char	*msh_dict(char const *const key, void *dict_data)
{
	t_cd *const	pcd = (t_cd *)dict_data;

	if (!ft_strcmp(key, "?"))
		return (ft_itoa(pcd->exit_status));
	else
		return (getenv(key));
}

bool	msh_isvalidvarchar(char const c, void *validchar_data)
{
	static char					firstc;
	static char const *const	special = "?";
	bool *const					isfirstc = (bool *)validchar_data;

	if (*isfirstc)
	{
		firstc = c;
		*isfirstc = false;
		if (ft_isdigit(c))
			return (false);
		if (ft_strchr(special, c))
			return (true);
	}
	else if (ft_strchr(special, firstc))
		return (false);
	return (ft_isalnum(c) || c == '_');
}

static char	**msh_expand_wordsplit_token(char const *const token,
	t_sqed const *const psqed)
{
	char 		*argword;
	char 		**args;
	int			argidx;
	t_qed		qed;
	t_qes		qes4wordsplit;

	qed = rh_sqed2qed(psqed);
	qed.expstat = false;
	qed.rmqe = false;
	argword = rh_rmqed(token, &qed);
	qes4wordsplit = rh_sqed2qes_ws(psqed);
	if (!qed.expstat)
		qes4wordsplit.blank = "";
	args = rh_split_quotes(argword, qes4wordsplit);
	free(argword);
	qed = rh_sqed2qed_rmonly(psqed);
	argidx = 0;
	while (args && args[argidx])
	{
		argword = rh_rmqed(args[argidx], &qed);
		free(args[argidx]);
		args[argidx++] = argword;
	}
	return (args);
}

static bool msh_ioredirect_from_fd(t_tca *ptca, const char *const token,
	char const **const piooperator, t_llong *pfd_new)
{
	int	 success;

	*pfd_new = ft_atoll_check_input(token, INT_MAX, &success);
	if (*pfd_new < 0  || success <= 0 || !ft_strchr("<>", token[success]))
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

static void	msh_ioredirect_settype(t_tca *ptca, char const *const iooperator, t_llong fd_new)
{
	if (!ft_strcmp(iooperator, ">"))
		ptca->curcmd->iolst->tredirect = (t_rd)fd2file_trunc;
	else if (!ft_strcmp(iooperator, ">>"))
		ptca->curcmd->iolst->tredirect = (t_rd)fd2file_append;
	else if (!ft_strcmp(iooperator, "<"))
		ptca->curcmd->iolst->tredirect = (t_rd)file2fd;
	else if (!ft_strcmp(iooperator, "<<"))
		ptca->curcmd->iolst->tredirect = (t_rd)heredoc2fd;
	if (fd_new == -1)
	{
		if ((ptca->curcmd->iolst->tredirect == (t_rd)fd2file_trunc)
			|| (ptca->curcmd->iolst->tredirect == (t_rd)fd2file_append))
			fd_new = 1;
		if ((ptca->curcmd->iolst->tredirect == (t_rd)file2fd)
			|| (ptca->curcmd->iolst->tredirect == (t_rd)heredoc2fd))
			fd_new = 0;
	}
	ptca->curcmd->iolst->new_fd = fd_new;	
}

static bool	msh_ioredirect_setiotarget(t_cd *pcd, t_tca *ptca,
	t_sqed const sqed, char const *const iotarget)
{
	char **const	token_ws = msh_expand_wordsplit_token(iotarget, &sqed);
	char 			buf[1024];	
	
	if (iotarget && ((ft_strlen(iotarget) == 0)
					|| !ft_strchr(sqed.metachr, iotarget[0])))
	{
		if (!token_ws || token_ws[0] == (char *) NULL
			|| token_ws[1] != (char *) NULL)
		{
			msh_error((char const*[]){iotarget,
				"ambiguous redirect"}, 2, -1);
			pcd->exit_status = 130;
			return (false);
		}
		ptca->curcmd->iolst->fname_delim = ft_strdup(token_ws[0]);
		return (rh_free_simple_array((void **)token_ws), true);
	}
	ft_sprintf(buf, "syntax error near unexpected token `%s'", iotarget);
	msh_error((char const*[]){buf, }, 1, -1);
	ft_sprintf(buf, "`%s'", ptca->tosplit);
	msh_error((char const*[]){buf, }, 1, -1);				
	pcd->exit_status = 2;
	return (false);
}

static bool msh_process_ioredirector(t_cd *pcd, t_tca *ptca,
	char const *const token, char const *const iotarget, t_sqed const sqed)
{
	char const	*iooperator;
	t_llong		fd_new;
	
	if (!get_iolstelem(ptca))
		return (false);
	iooperator = token;
	if (!msh_ioredirect_from_fd(ptca, token, &iooperator, &fd_new))
		return (false);
	// t_llong	fd_new = ft_atoll_check_input(token, INT_MAX, &success);
	// if (fd_new < 0  || success <= 0 || !ft_strchr("<>", token[success]))
	// {
	// 	fd_new = -1;
	// 	success = ft_strlen(token) - 1;
	// 	while (success > 0 && ft_strchr("<>", token[success - 1]))
	// 		--success;
	// 	if (!get_curarg(ptca))
	// 		return (false);
	// 	if (success > 0)
	// 	{
	// 		ptca->curarg->arg = ft_substr(token, 0, success);
	// 		iooperator = &(token[success]);
	// 	}
	// }
	msh_ioredirect_settype(ptca, iooperator, fd_new);
	// if (!ft_strcmp(iooperator, ">"))
	// 	ptca->curcmd->iolst->tredirect = (t_rd)fd2file_trunc;
	// else if (!ft_strcmp(iooperator, ">>"))
	// 	ptca->curcmd->iolst->tredirect = (t_rd)fd2file_append;
	// else if (!ft_strcmp(iooperator, "<"))
	// 	ptca->curcmd->iolst->tredirect = (t_rd)file2fd;
	// else if (!ft_strcmp(iooperator, "<<"))
	// 	ptca->curcmd->iolst->tredirect = (t_rd)heredoc2fd;
	// if (fd_new == -1)
	// {
	// 	if ((ptca->curcmd->iolst->tredirect == (t_rd)fd2file_trunc)
	// 		|| (ptca->curcmd->iolst->tredirect == (t_rd)fd2file_append))
	// 		fd_new = 1;
	// 	if ((ptca->curcmd->iolst->tredirect == (t_rd)file2fd)
	// 		|| (ptca->curcmd->iolst->tredirect == (t_rd)heredoc2fd))
	// 		fd_new = 0;
	// }
	// ptca->curcmd->iolst->new_fd = fd_new;
	if (!msh_ioredirect_setiotarget(pcd, ptca, sqed, iotarget))
		return (false);
	// if (iotarget && ((ft_strlen(iotarget) == 0)
	// 				|| !ft_strchr(sqed.metachr, iotarget[0])))
	// {
	// 	char **token_ws = msh_expand_wordsplit_token(iotarget, &sqed);
	// 	if (!token_ws || token_ws[0] == (char *) NULL
	// 		|| token_ws[1] != (char *) NULL)
	// 	{
	// 		msh_error((char const*[]){iotarget,
	// 			"ambiguous redirect"}, 2, -1);
	// 		pcd->exit_status = 130;
	// 		return (false);
	// 	}
	// 	ptca->curcmd->iolst->fname_delim = ft_strdup(token_ws[0]);
	// 	rh_free_simple_array((void **)token_ws);
	// }
	// else
	// {
	// 	char buf[1024];
	// 	ft_sprintf(buf, "syntax error near unexpected token `%s'", iotarget);
	// 	msh_error((char const*[]){buf, }, 1, -1);
	// 	ft_sprintf(buf, "`%s'", ptca->tosplit);
	// 	msh_error((char const*[]){buf, }, 1, -1);				
	// 	pcd->exit_status = 2;
	// 	return (false);
	// }
	if ((ptca->curcmd->iolst->tredirect == (t_rd)rd_default)
		|| (!ptca->curcmd->iolst->fname_delim))
	{
		pcd->exit_status = 130;
		msh_error((char const *[]){"Internal Error", }, 1, -1);
	}	
	return (true);
}

static int	msh_process_metachar(t_cd *pcd, t_tca *ptca,
	int *ptokenidx, t_sqed const sqed)
{
	int const	tokenidx = *ptokenidx;
	char const	*token = ptca->tokens[tokenidx];
	int	const	tokenlen = ft_strlen(token);
	
	if (!ft_strchr(sqed.metachr, token[tokenlen - 1]))
		return (0);
	if (!ft_strcmp(token, "|"))
		ptca->curcmd->ctrlchr = '|';
	else if (!ft_strcmp(token, "\n"))
		ptca->curcmd->ctrlchr = '\n';
	else if (ft_strchr("<>", token[tokenlen - 1]))
	{
		if (msh_process_ioredirector(pcd, ptca,
				token, ptca->tokens[tokenidx + 1], sqed))
			++(*ptokenidx);
		else
			return (-1);
	}
	return (1);
}

static bool	msh_process_simplecmd_token(t_cd *pcd, t_tca *ptca,
	int *ptokenidx, t_sqed const sqed)
{
	int		res;
	char	**args;

	res = msh_process_metachar(pcd, ptca, ptokenidx, sqed);
	if (res == -1)
		return (false);
	else if (res == 0)
	{
		args = msh_expand_wordsplit_token(ptca->tokens[*ptokenidx], &sqed);
		res = 0;
		while (args && args[res])
		{
			if (!get_curarg(ptca))
				return (false);
			ptca->curarg->arg =args[res++];
		}
	}
	return (true);
}

t_cmd	*msh_compile_cmds(t_cd *pcd, char const *const to_split,
			t_sqed const sqed)
{
	t_tca		tca;
	int			tokenidx;

	tca.tosplit = to_split;
	tca.tokens = rh_split_quotes(to_split, rh_sqed2qes(&sqed));
	tca.cmdlst = (t_cmd *) NULL;
	tca.arglst = (t_cmdarg *) NULL;
	tokenidx = 0;
	while (tca.tokens && tca.tokens[tokenidx])
	{
		if (!set_curcmd(&tca))
			return (cleanup_tca(&tca), NULL);
		if (!msh_process_simplecmd_token(pcd, &tca, &tokenidx, sqed))
			return (cleanup_tca(&tca), (t_cmd *) NULL);
		++tokenidx;
		if (tca.curcmd->ctrlchr || !tca.tokens[tokenidx])
		{
			if (!convert_arglst(&tca))
				return (cleanup_tca(&tca), (t_cmd *) NULL);
			cleanup_arglst(&tca);
		}
	}
	return (cleanup_tca(&tca), tca.cmdlst);
}
