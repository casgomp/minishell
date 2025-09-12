/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 11:30:12 by rhaas             #+#    #+#             */
/*   Updated: 2025/05/20 09:49:51 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char	*sc1;
	const unsigned char	*sc2;

	if (n == 0)
		return (0);
	sc1 = (const unsigned char *)s1;
	sc2 = (const unsigned char *)s2;
	while ((*sc1 == *sc2) && --n)
	{
		++sc1;
		++sc2;
	}
	return (((*sc1) - (*sc2)));
}
