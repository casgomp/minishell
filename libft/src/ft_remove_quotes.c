/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_remove_quotes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raphha <raphha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 17:57:26 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/12 16:55:50 by raphha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_rmquotes_expand(char *const qutdstr,
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
		if ((!actvq && ft_strchr(q, *pc)) || (actvq && *pc == actvq))
		{
			if (!actvq)
				actvq = *pc;
			else if (actvq)
				actvq = '\0';
			ft_strlcpy(pc, pc + 1, ft_strlen(pc));
		}
		else
		{
 			if (esc == *pc && actvq == '"' && actvq == *(pc + 1))			
				ft_strlcpy(pc, pc + 1, ft_strlen(pc));
			++idx;
		}
	}
}
