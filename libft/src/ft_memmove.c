/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 22:19:10 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/22 13:31:16 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stddef.h>

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char		*local_dest;
	const unsigned char	*local_src;

	if (dest == src || n == 0)
		return (dest);
	if (dest || src)
	{
		local_dest = dest;
		local_src = src;
		if (local_dest < local_src)
			ft_memcpy(dest, src, n);
		else if (local_dest > local_src)
			while (n--)
				*(local_dest + n) = *(local_src + n);
	}
	return (dest);
}
