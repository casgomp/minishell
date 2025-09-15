/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pecastro <pecastro@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 12:07:30 by pecastro          #+#    #+#             */
/*   Updated: 2025/07/24 12:07:34 by pecastro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_function.h"
#include "ft_string.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char	*ptr;
	size_t	size;
	size_t	i;

	size = ft_strlen((const char *)s);
	ptr = malloc(sizeof(char) * (size + 1));
	if (ptr == NULL)
	{
		return (NULL);
	}
	i = 0;
	while (i < size)
	{
		ptr[i] = f(i, s[i]);
		i ++;
	}
	ptr[i] = '\0';
	return (ptr);
}
