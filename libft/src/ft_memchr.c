/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 11:46:03 by rhaas             #+#    #+#             */
/*   Updated: 2025/05/10 11:51:47 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

void	*ft_memchr(const void *s, int c, size_t n)
{
	const unsigned char	ch = c;
	const unsigned char	*sc;

	sc = (const unsigned char *)s;
	while (n--)
	{
		if ((*sc) == ch)
			return ((void *)sc);
		else
			++sc;
	}
	return (NULL);
}
