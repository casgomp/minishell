/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 11:23:52 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/22 16:36:17 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

char	*ft_strrchr(const char *s, int c)
{
	char const	ch = c;
	char		*sc;

	if (!s)
		return ((char *) NULL);
	sc = (char *) NULL;
	while (*s)
	{
		if ((*s) == ch)
			sc = (char *)s;
		++s;
	}
	if (ch == '\0')
		sc = (char *)s;
	return (sc);
}
