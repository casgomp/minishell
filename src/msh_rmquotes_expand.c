/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_rmquotes_expand.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:12:30 by rhaas             #+#    #+#             */
/*   Updated: 2025/10/09 18:11:09 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

t_cmd	*msh_compile_cmds(char const *const to_split, t_sqed const sqed)
{
	char **const	tokens = rh_split_quotes(to_split,
			(t_qes){.q = sqed.q, .esc = sqed.esc,
			.blank = sqed.blank, .metachr = sqed.metachr});
	int				tokenidx;
	t_cmd			*cmdlst;
	t_cmd			*curcmd;
	t_cmdarg		*arglst;
	t_cmdarg		*curarg;
	int				num_args;
	int				argidx;

	cmdlst = (t_cmd *) NULL;
	arglst = (t_cmdarg *) NULL;
	tokenidx = 0;
	while (tokens && tokens[tokenidx])
	{		
		char const	*token = tokens[tokenidx];		
		int			tokenlen = ft_strlen(token);	
		
		// get curcmd function
		if (!cmdlst || curcmd->ctrlchr)
		{
			if (!cmdlst)
			{
				cmdlst = (t_cmd *)malloc(sizeof(t_cmd));
				curcmd = cmdlst;
			}
			else
			{
				curcmd->next = (t_cmd *)malloc(sizeof(t_cmd));
				curcmd = curcmd->next;
			}
			if (!curcmd)
				return (/* cleanup! */ (t_cmd *) NULL);
			*curcmd = (t_cmd){.stdio={-1, -1, -1}, .list_io=(t_io*)NULL, .args=(char **)NULL,
								.ctrlchr='\0', .next=(t_cmd *)NULL};
			num_args = 0;
		}
		// check for metachars:
		if (ft_strchr(sqed.metachr, tokens[tokenidx][tokenlen - 1]))
		{
			if (!ft_strcmp(token, "|"))
			{
				curcmd->ctrlchr = '|';
			}
			else
			{
				curcmd->list_io = malloc(sizeof(t_io));
				if (!curcmd->list_io)
					return (/* cleanup?, */ NULL);
				*(curcmd->list_io) =
					(t_io){.fname_delim = (char*) NULL, .new_fd = -1, .old_fd = -1,
						.tredirect = (t_rd)rd_default, .next = (t_io *)NULL};
				int success;
				t_llong fd_new = ft_atoll_check_input(token, INT_MAX, &success);
				if (success > 0)
				{
					// if (fd_new >= 1024)
					// 	fd_new = -1;
					//  ----> needs to be handled later as an error:
					//  "bash: $(fdnew): Bad file descriptor"

					// will be set later
					// curcmd->list_io->new_fd = fd_new;
					token = &(token[success]);
					tokenlen = ft_strlen(token);
				}
				else
					fd_new = -1;
				if (!ft_strcmp(token, ">"))
					curcmd->list_io->tredirect = (t_rd)fd2file_trunc;
				else if (!ft_strcmp(token, ">>"))
					curcmd->list_io->tredirect = (t_rd)fd2file_append;
				else if (!ft_strcmp(token, "<"))
					curcmd->list_io->tredirect = (t_rd)file2fd;
				else if (!ft_strcmp(token, "<<"))
					curcmd->list_io->tredirect = (t_rd)heredoc2fd;
			
				// Handle error later...
				// if ((curcmd->list_io->tredirect == (t_rd)rd_default)
				// 	|| (!curcmd->list_io->fname_delim))
				// {
				// 	// return (error)
				// }
				if (fd_new == -1)
				{
					if ((curcmd->list_io->tredirect == (t_rd)fd2file_trunc)
						|| (curcmd->list_io->tredirect == (t_rd)fd2file_append))
						fd_new = 1;
					if ((curcmd->list_io->tredirect == (t_rd)file2fd)
						|| (curcmd->list_io->tredirect == (t_rd)heredoc2fd))
						fd_new = 0;
				}
				// also saves the fd_new possibly created via ft_atoll!
				curcmd->list_io->new_fd = fd_new;
							
				if (tokens[tokenidx + 1]
					&& ((ft_strlen(tokens[tokenidx + 1]) == 0)
						|| !ft_strchr(sqed.metachr, tokens[tokenidx + 1][0])))
				{
					curcmd->list_io->fname_delim = ft_strdup(tokens[tokenidx + 1]);
				}
				else
				{
					// msh_error("bash: syntax error near unexpected token `%s'",
					// 	tokens[tokenidx + 1]);
					ft_printf("fixme: syntax error near unexpected token `%s'\n",
						tokens[tokenidx + 1]);
					return (t_cmd *) NULL;
				}
				++tokenidx;
			}
		}
		else // if not metachar or redirect group
		{
			// get curarg function
			{
				if (!arglst)
				{
					arglst = malloc(sizeof(t_cmdarg));
					curarg = arglst;
				}
				else
				{
					curarg->next = malloc(sizeof(t_cmdarg));
					curarg = curarg->next;
				}
				if (!curarg)
					return (/* cleanup, */ (t_cmd *) NULL);
				*curarg = (t_cmdarg){.arg = (char *) NULL, .next = (t_cmdarg *) NULL};			
				++num_args;
			}
			curarg->arg = ft_strdup(tokens[tokenidx]);
		}
		++tokenidx;
		if (curcmd->ctrlchr || !tokens[tokenidx])
		{
			// Transmogrify the args list
			// + param expansion
			// + word splitting
			// + quote removal
			// fill the args char** array
			curcmd->args = malloc((num_args + 1) * sizeof(char *));
			if (!curcmd->args)
				return (/* cleanup, */ (t_cmd *) NULL);
			curcmd->args[num_args] = (char *) NULL;
			argidx = -1;
			while (++argidx < num_args)
			{
				curcmd->args[argidx] = arglst->arg;
				curarg = arglst;
				arglst = arglst->next;
				free(curarg);
			}
			arglst = (t_cmdarg *) NULL;
		}
	}
	return (/* cleanup tokens, */ cmdlst);
}
