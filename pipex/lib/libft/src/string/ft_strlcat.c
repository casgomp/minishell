/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pecastro <pecastro@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 12:01:36 by pecastro          #+#    #+#             */
/*   Updated: 2025/07/24 12:01:39 by pecastro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_string.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	j;
	size_t	k;

	i = 0;
	while (dst[i] && i < size)
		i ++;
	j = 0;
	while (src[j])
		j ++;
	if (i == size)
		return (size + j);
	k = 0;
	while (src[k] && (k + i + 1) < size)
	{
		dst[i + k] = src[k];
		k ++;
	}
	if (i + k < size)
		dst[i + k] = '\0';
	return (i + j);
}
