/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_dprintf.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pecastro <pecastro@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 13:31:41 by pecastro          #+#    #+#             */
/*   Updated: 2025/08/09 16:19:27 by pecastro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_dprintf.h"

int	ft_putnbr_dprintf(int nb, int fd)
{
	char	c;
	int		count_chars;

	count_chars = 0;
	if (nb == -2147483648)
		return (ft_putstr_dprintf("-2147483648", fd));
	if (nb < 0)
	{
		count_chars += ft_putchar_dprintf('-', fd);
		nb = -nb;
	}
	if (nb >= 10)
		count_chars += ft_putnbr_dprintf(nb / 10, fd);
	c = (nb % 10) + '0';
	count_chars += ft_putchar_dprintf(c, fd);
	return (count_chars);
}
