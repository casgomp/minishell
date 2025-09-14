/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pecastro <pecastro@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 12:05:36 by pecastro          #+#    #+#             */
/*   Updated: 2025/07/24 12:05:39 by pecastro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"
#include "string.h"

char	*ft_substr(const char *s, unsigned int start, size_t len)
{
	char			*ptr;
	unsigned int	i;

	i = 0;
	if (s == NULL)
		return (NULL);
	if (start >= ft_strlen(s))
	{
		return (ft_strdup(""));
	}
	if (len >= (ft_strlen(s) - start))
	{
		len = ft_strlen(s) - start;
	}
	ptr = malloc(sizeof(char) * (len + 1));
	if (ptr == NULL)
		return (NULL);
	while (s[start + i] && (i < len))
	{
		ptr[i] = s[start + i];
		i ++;
	}
	ptr[i] = '\0';
	return (ptr);
}
