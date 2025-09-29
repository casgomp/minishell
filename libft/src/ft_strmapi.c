/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 10:49:17 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/22 15:56:31 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include <stddef.h>
#include <stdlib.h>

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int	idx;
	char			*s2;

	if (!(s && f))
		return ((char *) NULL);
	s2 = malloc((ft_strlen(s) + 1) * sizeof(char));
	if (s2)
	{
		idx = 0;
		while (s[idx])
		{
			s2[idx] = f(idx, s[idx]);
			++idx;
		}
		s2[idx] = '\0';
		return (s2);
	}
	return ((char *) NULL);
}
