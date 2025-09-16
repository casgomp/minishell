/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 14:25:20 by rhaas             #+#    #+#             */
/*   Updated: 2025/08/26 16:30:53 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <stddef.h>
#include "libft.h"

static int	ft_atoi_check_input_core(
	const char *nptr, const int sign, unsigned int limit, int *psuccess)
{
	char const *const	nptr0 = nptr;
	int					increment;
	unsigned int		result;
	int					overflow;

	result = 0;
	overflow = 1;
	if (*psuccess)
		*psuccess = 0;
	if (sign == -1)
		++limit;
	while ('0' <= (*nptr) && (*nptr) <= '9')
	{
		increment = (int)((*nptr) - '0');
		if (psuccess && result > (limit - increment) / 10)
			overflow = -1;
		result = result * 10 + increment;
		++nptr;
	}
	if (psuccess)
		*psuccess = overflow * ((nptr - nptr0) + (sign != 1));
	if (sign == -1)
		result *= -1;
	return (result);
}

int	ft_atoi_check_input(const char *nptr, int *psuccess)
{
	int	sign;

	sign = 1;
	if (psuccess)
		*psuccess = 0;
	if (nptr && *nptr)
	{
		while (*nptr == ' ' || ('\t' <= *(nptr) && *(nptr) <= '\r'))
			++nptr;
		if (*(nptr) == '+')
			sign = 2;
		else if (*(nptr) == '-')
			sign = -1;
		if (sign != 1)
			++nptr;
		if (ft_isdigit(*nptr))
		{
			return (ft_atoi_check_input_core(nptr, sign, INT_MAX, psuccess));
		}
	}
	return (0);
}

int	ft_atoi(const char *nptr)
{
	return (ft_atoi_check_input(nptr, (int *) NULL));
}
