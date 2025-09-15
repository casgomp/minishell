/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_rmquotes_expand.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 17:57:26 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/15 20:04:43 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_rmquotes_expand.h"
#include "libft.h"

#include <stdbool.h>
#include <stdlib.h>

// minishell_isAllowedBashVariableCharacter
int	ms_isbvc(char const c, bool firstc)
{
	if (firstc && ft_isdigit(c))
		return (false);
	return (ft_isalnum(c) || c == '_');
}

static int	set_tsstl(t_unqut_expd *pue, char *const pcstart)
{
	if (!(pue->tsl_orig) || (pue->tsl)->len)
	{
		if (!(pue->tsl_orig))
		{
			pue->tsl_orig = malloc(sizeof(t_strstart_len));
			pue->tsl = pue->tsl_orig;
		}
		else
		{
			pue->uestrlen += pue->tsl->len;
			pue->tsl->next = malloc(sizeof(t_strstart_len));
			pue->tsl = pue->tsl->next;
		}
		*(pue->tsl) = (t_strstart_len){pcstart, 0, (t_strstart_len *) NULL};
	}
	return (pue->tsl != (t_strstart_len *) NULL);
}

// Removes one level of quotes from the quoted string 'qutdstr'
// Different quote chars can be specified in the C-string 'q'
// (Only) if the quote uses the first char of 'q', escapes will be active
// and shell parameter expansion will happen.
// Most common values are: q="\"'" and esc='\\'
char	*ms_rmquotes_expand(char const *const qutdstr,
			char const *const q, char const esc)
{
	int				idx;
	char			*pc;
	t_unqut_expd	uedata;

	uedata = (t_unqut_expd){ft_substr(qutdstr, 0, ft_strlen(qutdstr)),
				(char *)NULL, '\0',
				(t_strstart_len *)NULL, (t_strstart_len *)NULL, 0};
	if (!(uedata.in && q && set_tsstl(&uedata, uedata.in))) 
		return ((char *) NULL);
	idx = 0;
	while (uedata.in[idx])
	{
		pc = &(uedata.in[idx]);
		if ((!uedata.actvq && ft_strchr(q, *pc))
			|| (uedata.actvq && *pc == uedata.actvq))
		{
			if (!uedata.actvq)
				uedata.actvq = *pc;
			else if (uedata.actvq)
				uedata.actvq = '\0';
			ft_strlcpy(pc, pc + 1, ft_strlen(pc));
			continue;
		}
		else if (uedata.actvq == q[0] || !uedata.actvq)
		{
			if ((esc == *pc)
				&& ((!uedata.actvq) || (uedata.actvq == *(pc + 1))))
			{
				ft_strlcpy(pc, pc + 1, ft_strlen(pc));
				// ++idx;
				// ++(uedata.tsl->len);
			}
			else if ('$' == *pc/*  && *(pc + 1) && !ms_isbvc(*(pc + 1)) */)
			{
				int paramlen = 1; // including '$'
				while (pc[paramlen] && ms_isbvc(pc[paramlen], 1 == paramlen) 
			 			/* && pc[paramlen] != q[0] -> ist schon durch ms_isbvc abgedeckt*/)
					++paramlen;
				if (paramlen > 1)
				{
					char *envvar = ft_substr(pc, 1, paramlen - 1);
					if (envvar/*  && *envvar */)
					{
						char *envval = getenv(envvar);
						if (envval)
						{
							set_tsstl(&uedata, envval);
							uedata.tsl->len = ft_strlen(envval);
						}
						free(envvar);
					}
					idx += paramlen;
					set_tsstl(&uedata, &(uedata.in[idx]));					
					continue;
				}
			}
		}
		// else
		{
			++idx;
			++(uedata.tsl->len);
		}
	}
	uedata.uestrlen += uedata.tsl->len;
	uedata.uestr = malloc((uedata.uestrlen + 1) *sizeof(char));
	uedata.tsl = uedata.tsl_orig;
	idx = 0;
	while (uedata.tsl)
	{
		ft_strlcpy(&(uedata.uestr[idx]), uedata.tsl->pcstart, uedata.tsl->len + 1);
		idx += uedata.tsl->len;
		uedata.tsl = uedata.tsl->next;
	}
	return (uedata.uestr);
}

void	ms_expand()
{};