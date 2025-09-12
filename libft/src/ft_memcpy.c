/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 22:08:14 by rhaas             #+#    #+#             */
/*   Updated: 2025/05/21 19:45:53 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char		*local_dest;
	const unsigned char	*local_src;

	if (dest || src)
	{
		local_dest = dest;
		local_src = src;
		while (n--)
			*(local_dest++) = *(local_src++);
	}
	return (dest);
}
