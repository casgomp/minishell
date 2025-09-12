/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 12:02:12 by rhaas             #+#    #+#             */
/*   Updated: 2025/05/19 15:21:16 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	const size_t	little_len = ft_strlen(little);
	size_t			lile;

	if ((*little) == '\0')
		return ((char *)big);
	while ((*big) && len)
	{
		if (*big == *little)
		{
			lile = 0;
			while ((lile < little_len) && (*(big + lile) == *(little + lile))
				&& (len--))
				++lile;
			if (lile == little_len)
				return ((char *)big);
			else if (len == 0)
				break ;
			else
				len += lile;
		}
		++big;
		--len;
	}
	return ((char *) NULL);
}
