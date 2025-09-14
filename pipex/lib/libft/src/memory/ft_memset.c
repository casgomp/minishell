/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pecastro <pecastro@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 12:04:07 by pecastro          #+#    #+#             */
/*   Updated: 2025/07/24 12:04:09 by pecastro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_memory.h"

void	*ft_memset(void *s, int c, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		((unsigned char *)s)[i] = c;
		i ++;
	}
	return (s);
}
