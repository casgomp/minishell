/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 20:41:22 by rhaas             #+#    #+#             */
/*   Updated: 2025/05/10 21:06:14 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stddef.h>

static int	char_in_set(char const c, char const *set)
{
	while (*set)
	{
		if (c == *set)
			return (1);
		++set;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t			len;
	unsigned int	start;

	len = ft_strlen(s1);
	start = 0;
	while (char_in_set(s1[start], set))
	{
		++start;
		--len;
	}
	while (len > 0 && char_in_set(s1[start + len - 1], set))
		--len;
	return (ft_substr(s1, start, len));
}
