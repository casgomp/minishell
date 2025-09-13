/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_rmquotes_expand.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 17:57:26 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/13 16:16:23 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include <stdlib.h>

int	ms_isspace(char const c)
{
	return (c == ' ' || ('\t' <= c && c <= '\r'));
}

// Removes one level of quotes from the quoted string 'qutdstr'
// Different quote chars can be specified in the C-string 'q'
// (Only) if the quote uses the first char of 'q', escapes will be active
// and shell parameter expansion will happen.
// Most common values are: q="\"'" and esc='\\'
void	ms_rmquotes_expand(char *const qutdstr,
			char const *const q, char const esc)
{
	int		idx;
	char	*pc;
	char	actvq;

	if (!(qutdstr && q)) 
		return (void);
	idx = 0;
	actvq = '\0';
	while (qutdstr[idx])
	{
		pc = &(qutdstr[idx]);
		if ((!actvq && ft_strchr(q, *pc)) || (actvq && *pc == actvq))
		{
			if (!actvq)
				actvq = *pc;
			else if (actvq)
				actvq = '\0';
			ft_strlcpy(pc, pc + 1, ft_strlen(pc));
		}
		else if (actvq == q[0])
		{
 			if (esc == *pc /* && actvq == q[0] */ && actvq == *(pc + 1))
			{
				ft_strlcpy(pc, pc + 1, ft_strlen(pc));
				++idx;
			}
			else if ('$' == *pc/*  && *(pc + 1) && *(pc + 1) != q[0] */)
			{
				int expansionstrlen = 1;
				while (pc[expansionstrlen] && pc[expansionstrlen] != q[0] && !ms_isspace(pc[expansionstrlen]))
					++expansionstrlen;
				if (expansionstrlen > 1)
				{
					char *envvar = ft_substr(pc, idx + 1, expansionstrlen - 1);
					if (envvar && *envvar)
					{
						// returns NULL if envvar not found; then expand to the empty string
						char const *const val = getenv(envvar);
						size_t const remainingstrlen = ft_strlen(pc);
						size_t const envvarlen = ft_strlen(envvar);
						size_t const vallen = ft_strlen(val);
						size_t const newstrlen = remainingstrlen - expansionstrlen;
						free(envvar);
					}
				}
			}
		}
		else
			++idx;
	}
}

void	ms_expand()
{};