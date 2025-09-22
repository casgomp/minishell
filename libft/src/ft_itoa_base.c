/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 21:58:48 by rhaas             #+#    #+#             */
/*   Updated: 2025/05/30 14:42:34 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static size_t	num_digits(intptr_t nbr, const size_t baselength)
{
	size_t	len;

	len = 1;
	nbr /= baselength;
	while (nbr)
	{
		nbr /= baselength;
		++len;
	}
	return (len);
}

static uintptr_t	uiabs(const intptr_t n, int *sign)
{
	uintptr_t	nb;

	nb = (uintptr_t)n;
	*sign = 0;
	if (n < 0)
	{
		*sign = -1;
		if (n > INTPTR_MIN)
			nb = -1 * n;
	}
	return (nb);
}

char	*ft_itoa_base(int n, const char *const base, int issigned)
{
	if (issigned)
		return (ft_ptrtoa_base((signed int)n, base, issigned));
	else
		return (ft_ptrtoa_base((unsigned int)n, base, issigned));
}

char	*ft_ptrtoa_base(intptr_t ptrval, const char *const base, int issigned)
{
	char			*numstr;
	int				sign;
	uintptr_t		nb;
	size_t			slen;
	const size_t	baselen = ft_strlen(base);

	sign = 0;
	nb = (uintptr_t)ptrval;
	if (issigned)
		nb = uiabs(ptrval, &sign);
	slen = num_digits(nb, baselen) - sign;
	numstr = malloc((slen + 1) * sizeof(char));
	if (!numstr)
		return ((char *) NULL);
	numstr[slen] = '\0';
	while ((slen--) + sign)
	{
		numstr[slen] = base[nb % baselen];
		nb /= baselen;
	}
	if (sign == -1)
		numstr[0] = '-';
	return (numstr);
}
