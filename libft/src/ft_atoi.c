/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raphha <raphha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 14:25:20 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/12 14:38:08 by raphha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <stddef.h>
#include "libft.h"

static t_llong	ft_atoll_check_input_core(
	const char *nptr, const int sign, t_ullong limit, int *psuccess)
{
	char const *const	nptr0 = nptr;
	int					increment;
	t_ullong			result;
	int					overflow;

	result = 0;
	overflow = 1;
	if (psuccess)
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

t_llong	ft_atoll_check_input(const char *nptr, t_ullong limit, int *psuccess)
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
			return (ft_atoll_check_input_core(nptr, sign, limit, psuccess));
		}
	}
	return (0LL);
}

int	ft_atoi(const char *nptr)
{
	return ((int)ft_atoll_check_input(nptr, INT_MAX, (int *) NULL));
}

int	ft_atoll(const char *nptr)
{
	return (ft_atoll_check_input(nptr, LLONG_MAX, (int *) NULL));
}
