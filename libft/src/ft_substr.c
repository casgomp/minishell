/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 20:13:13 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/02 17:38:50 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <limits.h>

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	const size_t	slen = ft_strlen(s);
	char			*sub;

	if (start >= slen)
		len = 0;
	else if (len > slen - start)
		len = slen - start;
	sub = malloc((len + 1) * sizeof(char));
	if (sub)
	{
		sub[len] = '\0';
		while (len--)
			sub[len] = s[start + len];
		return (sub);
	}
	return ((char *) NULL);
}
