/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_rmquotes_expand.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 17:57:26 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/15 21:21:40 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_rmquotes_expand.h"
#include "libft.h"

#include <stdbool.h>
#include <stdlib.h>

// minishell_isAllowedBashVariableCharacter
// https://thelinuxcode.com/bash-variable-name-rules-legal-illegal/
bool	ms_isbvc(char const c, bool firstc)
{
	(void)firstc;
	// static char first_is_digit;
	if (firstc)
	// {
	// 	first_is_digit = '\0';
	 	if (ft_isdigit(c))
	// 		first_is_digit = c;
	// }
	// else if (first_is_digit)
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
char	*ms_rmquotes_expand(char const *const qutdstr, t_qed tqed)
{
	int				idx;
	char			*pc;
	t_unqut_expd	uedata;

	uedata = (t_unqut_expd){ft_substr(qutdstr, 0, ft_strlen(qutdstr)),
				(char *)NULL, '\0',
				(t_strstart_len *)NULL, (t_strstart_len *)NULL, 0};
	if (!(uedata.in && tqed.q && set_tsstl(&uedata, uedata.in))) 
		return ((char *) NULL);
	idx = 0;
	while (uedata.in[idx])
	{
		pc = &(uedata.in[idx]);
		if ((!uedata.actvq && ft_strchr(tqed.q, *pc))
			|| (uedata.actvq && *pc == uedata.actvq))
		{
			if (!uedata.actvq)
				uedata.actvq = *pc;
			else if (uedata.actvq)
				uedata.actvq = '\0';
			ft_strlcpy(pc, pc + 1, ft_strlen(pc));
			continue;
		}
		else if (uedata.actvq == tqed.q[0] || !uedata.actvq)
		{
			if ((tqed.esc == *pc)
				&& ((!uedata.actvq) || (uedata.actvq == *(pc + 1))))
			{
				ft_strlcpy(pc, pc + 1, ft_strlen(pc));
			}
			else if (tqed.expnd == *pc)
			{
				int paramlen = 1; // including '$'
				while (pc[paramlen] && ms_isbvc(pc[paramlen], 1 == paramlen))
					++paramlen;
				if (paramlen > 1)
				{
					char *envvar = ft_substr(pc, 1, paramlen - 1);
					if (envvar)
					{
						// if (paramlen == 2 && ft_isdigit(*envvar))
						// {
						// 	int digit = (*envvar) - '0';
						// 	if (digit < argc)
						// 	{
						// 		free(envvar);
						// 		envvar = argv[digit];
						// 	}
						// 	else
						// 		envvar = "\0";
						// }
						// else
						{
							char *envval = tqed.dict(envvar);
							if (envval)
							{
								set_tsstl(&uedata, envval);
								uedata.tsl->len = ft_strlen(envval);
							}
							free(envvar);
						}
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
		free(uedata.tsl_orig);
		uedata.tsl_orig = uedata.tsl;
	}
	free(uedata.in);
	return (uedata.uestr);
}

void	ms_expand()
{};