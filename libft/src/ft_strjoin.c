/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 20:13:13 by rhaas             #+#    #+#             */
/*   Updated: 2025/05/16 11:27:18 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strjoin(char const *s1, char const *s2)
{
	const size_t	len = ft_strlen(s1) + ft_strlen(s2);
	char			*s;

	s = malloc((len + 1) * sizeof(char));
	if (s)
	{
		while (*s1)
			*(s++) = *(s1++);
		while (*s2)
			*(s++) = *(s2++);
		*s = '\0';
		return (s - len);
	}
	return ((char *) NULL);
}
