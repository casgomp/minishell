/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 21:58:48 by rhaas             #+#    #+#             */
/*   Updated: 2025/05/20 14:42:43 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <stddef.h>
#include <stdlib.h>

static size_t	num_digits(unsigned int nbr, const size_t baselength)
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

static unsigned int	uiabs(const int n, int *sign)
{
	unsigned int	nb;

	nb = (unsigned int)n;
	*sign = 1;
	if (n < 0)
	{
		*sign = -1;
		if (n > INT_MIN)
			nb = -1 * n;
	}
	return (nb);
}

char	*ft_itoa(int n)
{
	char			*numstr;
	int				sign;
	unsigned int	nb;
	size_t			slen;

	nb = uiabs(n, &sign);
	slen = num_digits(nb, 10);
	if (sign == -1)
		++slen;
	numstr = malloc((slen + 1) * sizeof(char));
	if (numstr)
	{
		numstr[slen] = '\0';
		while (slen--)
		{
			numstr[slen] = (nb % 10) + '0';
			nb /= 10;
		}
		if (sign == -1)
			numstr[0] = '-';
		return (numstr);
	}
	return ((char *) NULL);
}
