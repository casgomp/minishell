/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 11:00:42 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/04 17:42:29 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

char	*ft_strchr(const char *s, int c)
{
	char const	ch = (char)c;
	char		*sc;

	sc = (char *)s;
	while (*sc)
	{
		if ((*sc) == ch)
			return (sc);
		else
			++sc;
	}
	if (ch == '\0')
		return (sc);
	return ((char *) NULL);
}
