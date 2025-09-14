/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pecastro <pecastro@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 12:05:11 by pecastro          #+#    #+#             */
/*   Updated: 2025/07/24 12:05:13 by pecastro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_alloc.h"
#include "ft_string.h"

char	*ft_strdup(const char *s)
{
	char	*ptr;
	int		i;

	ptr = malloc(sizeof(char) * (ft_strlen((const char *)s) + 1));
	if (ptr == NULL)
	{
		return (NULL);
	}
	i = 0;
	while (s[i])
	{
		ptr[i] = s[i];
		i ++;
	}
	ptr[i] = '\0';
	return (ptr);
}
