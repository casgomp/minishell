/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_remove_quotes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raphha <raphha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 17:57:26 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/12 14:29:38 by raphha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_split_quotes.h"

#include <stdlib.h>

void	rmquotes_expand(char *const qutdstr,
			char const *const q, char const esc)
{
	int		idx;
	char	*pc;
	char	actvq;

	idx = 0;
	actvq = '\0';
	while (qutdstr[idx])
	{
		pc = &(qutdstr[idx]);
		if (*pc && ((actvq && *pc == actvq) || (!actvq && ft_strchr(q, *pc))))
		{
			if (!actvq)
				actvq = *pc;
			else if (actvq)
				actvq = '\0';
			ft_strlcpy(pc, pc + 1, ft_strlen(pc));
		}
		else if (*pc && esc == *pc && actvq == '"' && *(pc + 1) == actvq)
		{
			ft_strlcpy(pc, pc + 1, ft_strlen(pc));
			++idx;
		}
		else
			++idx;
	}
}
