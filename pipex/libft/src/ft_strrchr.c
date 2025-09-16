/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 11:23:52 by rhaas             #+#    #+#             */
/*   Updated: 2025/05/16 16:21:21 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

char	*ft_strrchr(const char *s, int c)
{
	char	ch;
	char	*sc;

	ch = (char)c;
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
