/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 21:35:56 by rhaas             #+#    #+#             */
/*   Updated: 2025/05/21 21:04:00 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char		*pc;
	const unsigned char	ch = c;

	pc = (unsigned char *)s;
	while (n--)
		*(pc++) = ch;
	return (s);
}
