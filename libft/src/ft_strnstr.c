/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 12:02:12 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/22 16:11:28 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	const size_t	little_len = ft_strlen(little);
	size_t			i;

	if (!(big && little))
		return ((char *) NULL);
	if ((*little) == '\0')
		return ((char *)big);
	while ((*big) && len)
	{
		if (*big == *little)
		{
			i = 0;
			while ((i < little_len) && (*(big + i) == *(little + i)) && (len--))
				++i;
			if (i == little_len)
				return ((char *)big);
			else if (len == 0)
				break ;
			else
				len += i;
		}
		++big;
		--len;
	}
	return ((char *) NULL);
}
