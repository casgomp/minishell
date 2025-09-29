/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 21:58:48 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/23 11:24:21 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static size_t	num_digits(t_llong nbr, const size_t baselength)
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

static t_ullong	ullong_abs(const t_llong n, int *sign)
{
	t_ullong	nb;

	nb = (t_ullong)n;
	*sign = 0;
	if (n < 0)
	{
		*sign = -1;
		if (n > LLONG_MIN)
			nb = -1 * n;
	}
	return (nb);
}

char	*ft_itoa_base(int n, const char *const base, bool issigned)
{
	if (issigned)
		return (ft_lltoa_base((signed int)n, base, issigned));
	else
		return (ft_lltoa_base((unsigned int)n, base, issigned));
}

char	*ft_lltoa_base(t_llong val, const char *const base, bool issigned)
{
	char			*numstr;
	int				sign;
	t_ullong		nb;
	size_t			slen;
	const size_t	baselen = ft_strlen(base);

	sign = 0;
	nb = (t_ullong)val;
	if (issigned)
		nb = ullong_abs(val, &sign);
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
