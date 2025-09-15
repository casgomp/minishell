/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_base_unsigned_dprintf.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pecastro <pecastro@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 12:04:17 by pecastro          #+#    #+#             */
/*   Updated: 2025/08/19 11:12:24 by pecastro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_dprintf.h"
#include "ft_string.h"

static int	ft_dprintf_rec(unsigned long long nb, char *base, int fd);
static int	ft_p(uintptr_t nb, int fd);

int	ft_putnbr_base_unsigned_dprintf(unsigned long long nb, 
		char converter, int fd)
{
	char	*base;
	int		count_chars;

	count_chars = 0;
	if (converter == 'u')
		base = "0123456789";
	else if (converter == 'p')
	{
		count_chars += ft_p((uintptr_t)nb, fd);
		if (nb == 0)
			return (count_chars);
		base = "0123456789abcdef";
	}
	else if (converter == 'x')
		base = "0123456789abcdef";
	else if (converter == 'X')
		base = "0123456789ABCDEF";
	count_chars += ft_dprintf_rec(nb, base, fd);
	return (count_chars);
}

static int	ft_dprintf_rec(unsigned long long nb, char *base, int fd)
{
	size_t	base_len;
	int		count_chars;

	base_len = ft_strlen(base);
	count_chars = 0;
	if (nb >= base_len)
		count_chars += ft_dprintf_rec(nb / base_len, base, fd);
	count_chars += ft_putchar_dprintf(base[nb % base_len], fd);
	return (count_chars);
}

static int	ft_p(uintptr_t nb, int fd)
{
	if (nb == 0)
		return (ft_putstr_dprintf("(nil)", fd));
	else
		return (ft_putstr_dprintf("0x", fd));
}
