/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 23:22:39 by rhaas             #+#    #+#             */
/*   Updated: 2025/05/20 11:28:01 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dsize)
{
	size_t			i;
	const size_t	dlen = ft_strlen(dst);

	if (dlen >= dsize)
		return (ft_strlen(src) + dsize);
	if (dsize > 0)
	{
		dst += dlen;
		dsize -= dlen;
		i = 0;
		while ((i < (dsize - 1)) && (src[i] != '\0'))
		{
			dst[i] = src[i];
			++i;
		}
		dst[i] = '\0';
	}
	return (ft_strlen(src) + dlen);
}
