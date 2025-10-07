/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_rmquotes_expand.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:12:30 by rhaas             #+#    #+#             */
/*   Updated: 2025/10/07 17:35:58 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	char **const	split = rh_split_quotes(to_split,
			(t_qes){.q = sqed.q, .esc = sqed.esc,
			.blank = sqed.blank, .metachr = sqed.metachr});
	char			*substr;
	int				idx;
	t_cmd			*tcmds;
	// t_cmd			*tccmd;

	tcmds = (t_cmd *) NULL;
	if (split)
	{
		tcmds = (t_cmd *)malloc(sizeof(t_cmd));
		if (!tcmds)
			return ((t_cmd *) NULL);
		*tcmds = (t_cmd){.stdio={-1, -1, -1}, .list_io=(t_io*)NULL, .args=(char **)NULL,
							.ctrlchr='\0', .next=(t_cmd *)NULL};
		idx = 0;
		while (split[idx])
		{
			substr = rh_rmqed(split[idx],
					(t_qed){.q = "", .esc = sqed.esc, .expnd = sqed.expnd,
					.dict = sqed.dict, .dict_data = sqed.dict_data,
					.validchar = sqed.validchar,
					.validchar_data = sqed.validchar_data});
			free(split[idx]);
			split[idx] = substr;
			++idx;
		}
	}
	return (tcmds);
}
