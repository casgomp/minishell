/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 21:08:22 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/04 17:36:41 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static size_t	section_count(char const *s, const char c)
{
	size_t	cnt;

	cnt = 0;
	while (*s)
	{
		if (*s && c != *s)
			++cnt;
		while (*s && c != *s)
			++s;
		while (*s && c == *s)
			++s;
	}
	return (cnt);
}

static char	**cleanup_strarr(char **split, size_t failidx)
{
	size_t	idx;

	idx = 0;
	while (idx < failidx)
	{
		free(split[idx]);
		++idx;
	}
	free(split);
	return ((char **) NULL);
}

char	**ft_split(char const *s, char c)
{
	char			**split;
	char			*sub;
	const size_t	subs = section_count(s, c);
	size_t			idx;

	split = (char **)malloc((subs + 1) * sizeof(char *));
	if (!split)
		return ((char **) NULL);
	split[subs] = (char *) NULL;
	idx = 0;
	while (idx < subs)
	{
		while (*s == c)
			++s;
		sub = ft_substr(s, 0, (ft_strchr(s, c) - s));
		if (!sub)
			return (cleanup_strarr(split, idx));
		split[idx++] = sub;
		s += ft_strlen(sub) + 1;
	}
	return (split);
}
