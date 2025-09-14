/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pecastro <pecastro@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 12:04:57 by pecastro          #+#    #+#             */
/*   Updated: 2025/07/24 12:04:59 by pecastro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_alloc.h"

static size_t	ft_len(int n)
{
	size_t	i;
	int		result;

	i = 0;
	result = n;
	if (n == 0)
	{
		return (1);
	}
	if (n < 0)
	{
		i ++;
	}
	while (result != 0)
	{
		result = result / 10;
		i ++;
	}
	return (i);
}

static void	ft_recur(int n, char *ptr, unsigned int *i, unsigned int j)
{
	if (n == -2147483648)
	{
		j = 0;
		while (j < 11)
		{
			ptr[(*i)++] = ("-2147483648"[j]);
			j ++;
		}
		return ;
	}
	if (n < 0)
	{
		ptr[(*i)++] = '-';
		n = -n;
	}
	if (n > 9)
	{
		ft_recur(n / 10, ptr, i, j);
	}
	ptr[(*i)++] = n % 10 + '0';
}

char	*ft_itoa(int n)
{
	char			*ptr;
	unsigned int	i;

	i = 0;
	ptr = malloc(sizeof(char) * (ft_len(n) + 1));
	if (ptr == NULL)
	{
		return (NULL);
	}
	ft_recur(n, ptr, &i, 0);
	ptr[i] = '\0';
	return (ptr);
}
