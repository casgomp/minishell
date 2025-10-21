/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_rmquotes_expand.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:12:30 by rhaas             #+#    #+#             */
/*   Updated: 2025/10/21 10:21:29 by rhaas            ###   ########.fr       */
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

static t_qed	rh_sqed2qed(t_sqed const *const psqed)
{
	return ((t_qed){
		.q = psqed->q, .esc = psqed->esc,
		.expnd = psqed->expnd, .ifs = psqed->ifs,
		.dict = psqed->dict, .dict_data = psqed->dict_data,
		.validchar = psqed->validchar, .validchar_data = psqed->validchar_data
	});
}

static t_qes	rh_sqed2qes(t_sqed const *const psqed)
{
	return ((t_qes){
		.q = psqed->q, .esc = psqed->esc,
		.blank = psqed->blank, .metachr = psqed->metachr		
	});
}

static bool	msh_compile_simplecmd(t_cd *pcd, t_tca *ptca, int tokenidx,
	t_sqed const sqed)
{
	t_qed const	qed = rh_sqed2qed(&sqed);	
	char const	*token = ptca->tokens[tokenidx];
	int			tokenlen = ft_strlen(token);
	// check for metachars:
	if (ft_strchr(sqed.metachr, token[tokenlen - 1]))
	{
		if (!ft_strcmp(token, "|"))
			ptca->curcmd->ctrlchr = '|';
		else if (!ft_strcmp(token, "\n"))
			ptca->curcmd->ctrlchr = '\n';
		else if (ft_strchr("<>", token[tokenlen - 1]))
		{
			if (!get_iolstelem(ptca))
				return (cleanup_tca(ptca), NULL);
			int success;
			t_llong	fd_new = ft_atoll_check_input(token, INT_MAX, &success);
			if (success <= 0 || fd_new < 0 || !ft_strchr("<>", token[success]))
			{
				fd_new = -1;
				success = tokenlen - 1;
				while (success > 0 && ft_strchr("<>", token[success - 1]))
					--success;
				if (!get_curarg(ptca))
					return (cleanup_tca(ptca), NULL);
				if (success > 0)
				{
					ptca->curarg->arg = ft_substr(token, 0, success);
					token = &(token[success]);
					tokenlen = ft_strlen(token);
				}
			}
			if (!ft_strcmp(token, ">"))
				ptca->curcmd->iolst->tredirect = (t_rd)fd2file_trunc;
			else if (!ft_strcmp(token, ">>"))
				ptca->curcmd->iolst->tredirect = (t_rd)fd2file_append;
			else if (!ft_strcmp(token, "<"))
				ptca->curcmd->iolst->tredirect = (t_rd)file2fd;
			else if (!ft_strcmp(token, "<<"))
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
						
			if (ptca->tokens[tokenidx + 1]
				&& ((ft_strlen(ptca->tokens[tokenidx + 1]) == 0)
					|| !ft_strchr(sqed.metachr, ptca->tokens[tokenidx + 1][0])))
			{
				// check string expansion
				// + not more than one word after wordsplit!
				char **wordsplit = rh_rmqed(ptca->tokens[tokenidx + 1], qed);
				if (!wordsplit || wordsplit[0] == (char *) NULL
					|| wordsplit[1] != (char *) NULL)
				{
					msh_error((char const*[]){ptca->tokens[tokenidx + 1], "ambiguous redirect"}, 2, -1);
					pcd->exit_status = 130;
					return (cleanup_tca(ptca), false);
				}
				ptca->curcmd->iolst->fname_delim
					= ft_strdup(ptca->tokens[tokenidx + 1]);
			}
			else
			{
				char buf[1024];
				ft_sprintf(buf, "syntax error near unexpected token `%s'", 
					ptca->tokens[tokenidx + 1]);
				msh_error((char const*[]){buf, }, 1, -1);
				ft_sprintf(buf, "`%s'", ptca->tosplit);
				msh_error((char const*[]){buf, }, 1, -1);				
				pcd->exit_status = 2;
				return (cleanup_tca(ptca), false);
			}
			// Handle error
			// if ((curcmd->iolst->tredirect == (t_rd)rd_default)
			// 	|| (!curcmd->iolst->fname_delim))
			// {
			// 	// return (error)
			// }
			++tokenidx;
		}
	}
	else // if not metachar or redirect group
	{
		char **wordsplit = rh_rmqed(ptca->tokens[tokenidx], qed);
		while (wordsplit && *wordsplit)
		{
			if (!get_curarg(ptca))
				return (cleanup_tca(ptca), (t_cmd *) NULL);
			ptca->curarg->arg = *(wordsplit++);
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
		// set new curcmd if necessary
		if (!set_curcmd(&tca))
			return (cleanup_tca(&tca), NULL);

		if (!msh_compile_simplecmd(pcd, &tca, tokenidx, sqed))
			return (cleanup_tca(&tca), (t_cmd *) NULL);
		// char const	*token = tca.tokens[tokenidx];		
		// int			tokenlen = ft_strlen(token);
		// // check for metachars:
		// if (ft_strchr(sqed.metachr, token[tokenlen - 1]))
		// {
		// 	if (!ft_strcmp(token, "|"))
		// 		tca.curcmd->ctrlchr = '|';
		// 	else if (!ft_strcmp(token, "\n"))
		// 		tca.curcmd->ctrlchr = '\n';
		// 	else if (ft_strchr("<>", token[tokenlen - 1]))
		// 	{
		// 		if (!get_iolstelem(&tca))
		// 			return (cleanup_tca(&tca), NULL);
		// 		int success;
		// 		t_llong	fd_new = ft_atoll_check_input(token, INT_MAX, &success);
		// 		if (success <= 0 || fd_new < 0 || !ft_strchr("<>", token[success]))
		// 		{
		// 			fd_new = -1;
		// 			success = tokenlen - 1;
		// 			while (success > 0 && ft_strchr("<>", token[success - 1]))
		// 				--success;
		// 			if (!get_curarg(&tca))
		// 				return (cleanup_tca(&tca), NULL);
		// 			if (success > 0)
		// 			{
		// 				tca.curarg->arg = ft_substr(token, 0, success);
		// 				token = &(token[success]);
		// 				tokenlen = ft_strlen(token);
		// 			}
		// 		}
		// 		if (!ft_strcmp(token, ">"))
		// 			tca.curcmd->iolst->tredirect = (t_rd)fd2file_trunc;
		// 		else if (!ft_strcmp(token, ">>"))
		// 			tca.curcmd->iolst->tredirect = (t_rd)fd2file_append;
		// 		else if (!ft_strcmp(token, "<"))
		// 			tca.curcmd->iolst->tredirect = (t_rd)file2fd;
		// 		else if (!ft_strcmp(token, "<<"))
		// 			tca.curcmd->iolst->tredirect = (t_rd)heredoc2fd;
		// 		if (fd_new == -1)
		// 		{
		// 			if ((tca.curcmd->iolst->tredirect == (t_rd)fd2file_trunc)
		// 				|| (tca.curcmd->iolst->tredirect == (t_rd)fd2file_append))
		// 				fd_new = 1;
		// 			if ((tca.curcmd->iolst->tredirect == (t_rd)file2fd)
		// 				|| (tca.curcmd->iolst->tredirect == (t_rd)heredoc2fd))
		// 				fd_new = 0;
		// 		}
		// 		tca.curcmd->iolst->new_fd = fd_new;		
		// 		if (tca.tokens[tokenidx + 1]
		// 			&& ((ft_strlen(tca.tokens[tokenidx + 1]) == 0)
		// 				|| !ft_strchr(sqed.metachr, tca.tokens[tokenidx + 1][0])))
		// 		{
		// 			// check string expansion
		// 			// + not more than one word after wordsplit!
		// 			char **wordsplit = rh_rmqed(tca.tokens[tokenidx + 1], qed);
		// 			if (!wordsplit || wordsplit[0] == (char *) NULL
		// 				|| wordsplit[1] != (char *) NULL)
		// 			{
		// 				msh_error((char const*[]){tca.tokens[tokenidx + 1], "ambiguous redirect"}, 2, -1);
		// 				pcd->exit_status = 130;
		// 				return (cleanup_tca(&tca), (t_cmd *) NULL);
		// 			}
		// 			tca.curcmd->iolst->fname_delim
		// 				= ft_strdup(tca.tokens[tokenidx + 1]);
		// 		}
		// 		else
		// 		{
		// 			char buf[1024];
		// 			ft_sprintf(buf, "syntax error near unexpected token `%s'", 
		// 				tca.tokens[tokenidx + 1]);
		// 			msh_error((char const*[]){buf, }, 1, -1);
		// 			ft_sprintf(buf, "`%s'", to_split);
		// 			msh_error((char const*[]){buf, }, 1, -1);				
		// 			pcd->exit_status = 2;
		// 			return (cleanup_tca(&tca), (t_cmd *) NULL);
		// 		}
		// 		// Handle error
		// 		// if ((curcmd->iolst->tredirect == (t_rd)rd_default)
		// 		// 	|| (!curcmd->iolst->fname_delim))
		// 		// {
		// 		// 	// return (error)
		// 		// }
		// 		++tokenidx;
		// 	}
		// }
		// else // if not metachar or redirect group
		// {
		// 	char **wordsplit = rh_rmqed(tca.tokens[tokenidx], qed);
		// 	while (wordsplit && *wordsplit)
		// 	{
		// 		if (!get_curarg(&tca))
		// 			return (cleanup_tca(&tca), (t_cmd *) NULL);
		// 		tca.curarg->arg = *(wordsplit++);
		// 	}
		// }
		++tokenidx;
		if (tca.curcmd->ctrlchr || !tca.tokens[tokenidx])
		{
			if (!convert_arglst(&tca))
				return (cleanup_tca(&tca), (t_cmd *) NULL);
			cleanup_arglst(&tca);
			// output parsing error if there is no command / empty args...
			// maybe a cmd that consists only of redirections??
		}
	}
	return (cleanup_tca(&tca), tca.cmdlst);
}
