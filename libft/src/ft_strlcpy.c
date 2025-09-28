/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 23:03:52 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/25 17:58:34 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dsize)
{
	size_t	i;
	size_t const originalsrclen = ft_strlen(src);

	if (dsize > 0)
	{
		i = 0;
		while ((i < (dsize - 1)) && (src[i] != '\0'))
		{
			dst[i] = src[i];
			++i;
		}
		dst[i] = '\0';
	}
	return (originalsrclen);
}
