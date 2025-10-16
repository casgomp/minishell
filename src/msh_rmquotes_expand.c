/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_rmquotes_expand.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:12:30 by rhaas             #+#    #+#             */
/*   Updated: 2025/10/16 12:39:34 by rhaas            ###   ########.fr       */
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

// minishell_isAllowedBashVariableCharacter
// https://thelinuxcode.com/bash-variable-name-rules-legal-illegal/
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

t_cmd	*msh_compile_cmds(t_cd *pcd, char const *const to_split, t_sqed const sqed)
{
	t_tca		tca;
	t_qes const	qes = (t_qes){.q = sqed.q, .esc = sqed.esc,
			.blank = sqed.blank, .metachr = sqed.metachr};
	t_qed const	qed = (t_qed){.q = sqed.q, .esc = sqed.esc, .expnd = sqed.expnd,
					.dict = sqed.dict, .dict_data = sqed.dict_data,
					.validchar = sqed.validchar,
					.validchar_data = sqed.validchar_data};
	int			tokenidx;

	tca.tokens = rh_split_quotes(to_split, qes);
	tca.cmdlst = (t_cmd *) NULL;
	tca.arglst = (t_cmdarg *) NULL;
	tokenidx = 0;
	while (tca.tokens && tca.tokens[tokenidx])
	{		
		char const	*token = tca.tokens[tokenidx];		
		int			tokenlen = ft_strlen(token);	
		
		// set new curcmd if necessary
		if (!set_curcmd(&tca))
			return (cleanup_tca(&tca), NULL);
		// check for metachars:
		if (ft_strchr(sqed.metachr, token[tokenlen - 1]))
		{
			if (!ft_strcmp(token, "|"))
			{
				tca.curcmd->ctrlchr = '|';
			}
			else
			{
				if (!get_iolstelem(&tca))
					return (cleanup_tca(&tca), NULL);
				int success;
				t_llong fd_new = ft_atoll_check_input(token, INT_MAX, &success);
				if (success > 0)
				{
					// if (fd_new >= 1024)
					// 	fd_new = -1;
					//  ----> needs to be handled later as an error:
					//  "bash: $(fdnew): Bad file descriptor"

					// will be set later
					// curcmd->iolst->new_fd = fd_new;
					token = &(token[success]);
					tokenlen = ft_strlen(token);
				}
				else
				{
					// evtl hier eine fehlgeschlagene Umwandlung von Zeichen vor den
					// redir operators in ein Arg umwandeln (statt schon in split)?
					fd_new = -1;
				}
				if (!ft_strcmp(token, ">"))
					tca.curcmd->iolst->tredirect = (t_rd)fd2file_trunc;
				else if (!ft_strcmp(token, ">>"))
					tca.curcmd->iolst->tredirect = (t_rd)fd2file_append;
				else if (!ft_strcmp(token, "<"))
					tca.curcmd->iolst->tredirect = (t_rd)file2fd;
				else if (!ft_strcmp(token, "<<"))
					tca.curcmd->iolst->tredirect = (t_rd)heredoc2fd;
				if (fd_new == -1)
				{
					if ((tca.curcmd->iolst->tredirect == (t_rd)fd2file_trunc)
						|| (tca.curcmd->iolst->tredirect == (t_rd)fd2file_append))
						fd_new = 1;
					if ((tca.curcmd->iolst->tredirect == (t_rd)file2fd)
						|| (tca.curcmd->iolst->tredirect == (t_rd)heredoc2fd))
						fd_new = 0;
				}
				// also saves the fd_new possibly created via ft_atoll!
				tca.curcmd->iolst->new_fd = fd_new;
							
				if (tca.tokens[tokenidx + 1]
					&& ((ft_strlen(tca.tokens[tokenidx + 1]) == 0)
						|| !ft_strchr(sqed.metachr, tca.tokens[tokenidx + 1][0])))
				{
					// check string expansion
					// + not more than one word after wordsplit!
					char **qedtoken = rh_rmqed(tca.tokens[tokenidx + 1], qed);
					char **wordsplit = qedtoken;
					// char **wordsplit = rh_split_quotes(qedtoken, qes);
					// rh_split_rmqed(tca.tokens[tokenidx + 1], sqed);
					if (wordsplit[0] == (char *) NULL || wordsplit[1] != (char *) NULL)
					{
						msh_error((char const*[]){tca.tokens[tokenidx + 1], "ambiguous redirect"}, 2, -1);
						//set return val $? = 130
						return (t_cmd *) NULL;
					}
					tca.curcmd->iolst->fname_delim
						= ft_strdup(tca.tokens[tokenidx + 1]);
				}
				else
				{
					char buf[1024];
					// msh_error("bash: syntax error near unexpected token `%s'",
					ft_sprintf(buf, "syntax error near unexpected token `%s'", 
						tca.tokens[tokenidx + 1]);
					// ft_dprintf(2, "syntax error near unexpected token `%s'", 
					// 	tca.tokens[tokenidx + 1]);
					msh_error((char const*[]){buf, }, 1, -1);
					ft_sprintf(buf, "`%s'", to_split);
					msh_error((char const*[]){buf, }, 1, -1);				
					pcd->exit_status = 2;
					return (t_cmd *) NULL;
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
			// tca.curarg->arg = ft_strdup(tca.tokens[tokenidx]);			
			// char *qedtoken = rh_rmqed(tca.tokens[tokenidx], qed);
			// // char **wordsplit = rh_split_quotes(qedtoken, qes);
			// char **wordsplit = (char *[]){qedtoken, NULL};
			char **qedtoken = rh_rmqed(tca.tokens[tokenidx], qed);
			char **wordsplit = qedtoken;
			while (*wordsplit)
			{
				if (!get_curarg(&tca))
					return (cleanup_tca(&tca), NULL);
				tca.curarg->arg = *(wordsplit++);
			}
		}
		++tokenidx;
		if (tca.curcmd->ctrlchr || !tca.tokens[tokenidx])
		{
			if (!convert_arglst(&tca))
				return (cleanup_tca(&tca), NULL);
			else
				cleanup_arglst(&tca);
		}
	}
	return (cleanup_tca(&tca), tca.cmdlst);
}
