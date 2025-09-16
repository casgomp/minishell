/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 14:42:52 by rhaas             #+#    #+#             */
/*   Updated: 2025/05/10 20:12:53 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t	numbytes;
	void	*ptr;

	numbytes = nmemb * size;
	if (size != 0 && numbytes / size != nmemb)
		return (NULL);
	ptr = malloc(numbytes);
	if (ptr && numbytes > 0)
	{
		while (numbytes--)
			((char *)ptr)[numbytes] = '\0';
	}
	return (ptr);
}
