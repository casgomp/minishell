/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 23:22:39 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/22 15:47:39 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dsize)
{
	size_t			idx;
	size_t const	dlen = ft_strlen(dst);
	size_t const	slen = ft_strlen(src);

	if (dlen >= dsize)
		return (slen + dsize);
	if (dsize > 0)
	{
		dst += dlen;
		dsize -= dlen;
		idx = 0;
		while ((idx < (dsize - 1)) && (src[idx] != '\0'))
		{
			dst[idx] = src[idx];
			++idx;
		}
		dst[idx] = '\0';
	}
	return (slen + dlen);
}
