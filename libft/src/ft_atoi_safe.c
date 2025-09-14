/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_safe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pecastro <pecastro@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 12:01:19 by pecastro          #+#    #+#             */
/*   Updated: 2025/08/21 14:54:59 by pecastro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_string.h"

int	ft_atoi_safe(const char *nptr, int *error)
{
	unsigned int	i;
	int				sign;
	long			result;

	*error = 0;
	sign = 1;
	result = 0;
	i = 0;
	while (((nptr[i] >= '\t') && (nptr[i] <= '\r')) || (nptr[i] == ' '))
		i++;
	while (nptr[i] == '+' || nptr[i] == '-')
	{
		if (nptr[i] == '-')
			sign = -sign;
		i ++;
	}
	result = ft_atoi_safe_digit(nptr, i, error, sign);
	if (*error)
		return (0);
	return ((int)result * sign);
}

unsigned int	ft_atoi_safe_digit(const char *nptr, 
						int i, int *error, int sign)
{
	int				digit;
	unsigned int	result;

	result = 0;
	digit = 0;
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		digit = nptr[i] - '0';
		if (sign == 1)
		{
			if (result > ((unsigned int)INT_MAX - digit) / 10)
				return (*error = 1, 0);
		}
		else
		{
			if (result > ((unsigned int)INT_MAX + 1 - digit) / 10)
				return (*error = 1, 0);
		}
		result = result * 10 + digit;
		i ++;
	}
	return (result);
}
