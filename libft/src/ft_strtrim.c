/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pecastro <pecastro@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 12:05:28 by pecastro          #+#    #+#             */
/*   Updated: 2025/07/24 12:05:31 by pecastro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_alloc.h"
#include "ft_string.h"

static int	ft_in_set(char const c, char const *set)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == c)
		{
			return (1);
		}
		i ++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	unsigned int	start;
	unsigned int	end;
	int				i;
	char			*ptr;

	if (s1 == NULL || set == NULL)
		return (NULL);
	if (s1[0] == '\0')
		return (ft_strdup(""));
	if (set[0] == '\0')
		return (ft_strdup(s1));
	start = 0;
	while (s1[start] && ft_in_set(s1[start], set))
		start ++;
	end = ft_strlen(s1);
	while (end > start && ft_in_set(s1[end - 1], set))
		end --;
	ptr = malloc(sizeof(char const) * (end - start + 1));
	if (ptr == NULL)
		return (NULL);
	i = 0;
	while (start < end)
		ptr[i++] = s1[start++];
	ptr[i] = '\0';
	return (ptr);
}
