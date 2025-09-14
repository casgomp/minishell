/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pecastro <pecastro@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 12:01:19 by pecastro          #+#    #+#             */
/*   Updated: 2025/08/21 14:49:21 by pecastro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_string.h"

int	ft_atoi(const char *nptr)
{
	unsigned int	i;
	int				sign;
	int				result;

	sign = 1;
	i = 0;
	while (((nptr[i] >= '\t') && (nptr[i] <= '\r')) || (nptr[i] == ' '))
		i++;
	while (nptr[i] == '+' || nptr[i] == '-')
	{
		if (nptr[i] == '-')
			sign = -sign;
		i ++;
	}
	result = 0;
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		result = (result * 10) + (nptr[i] - '0');
		i ++;
	}
	return (result * sign);
}
