/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pecastro <pecastro@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 16:34:13 by pecastro          #+#    #+#             */
/*   Updated: 2025/08/09 16:17:49 by pecastro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_dprintf.h"

static int	ft_is_converter(char c);
static int	ft_convert(char converter, va_list *ap, int fd);

int	ft_dprintf(int fd, const char *format, ...)
{
	va_list	ap;
	int		count_chars;
	int		i;

	if (format == NULL)
		return ('\0');
	va_start(ap, format);
	count_chars = 0;
	i = 0;
	while (format[i])
	{
		if (format[i] == '%' && ft_is_converter(format[i + 1]))
		{
			count_chars += ft_convert(format[i + 1], &ap, fd);
			i += 2;
		}
		else
			count_chars += ft_putchar_dprintf(format[i++], fd);
	}
	va_end(ap);
	return (count_chars);
}

static int	ft_is_converter(char c)
{
	int		i;
	char	*str;

	str = "cspdiuxX%";
	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (1);
		i ++;
	}
	return (0);
}

static int	ft_convert(char converter, va_list *ap, int fd)
{
	int	count_chars;

	count_chars = 0;
	if (converter == 'c')
		count_chars += ft_putchar_dprintf(va_arg(*ap, int), fd);
	else if (converter == 's')
		count_chars += ft_putstr_dprintf(va_arg(*ap, char *), fd);
	else if (converter == 'd' || converter == 'i')
		count_chars += ft_putnbr_dprintf(va_arg(*ap, int), fd);
	else if (converter == 'u' || converter == 'x' || converter == 'X')
		count_chars += ft_putnbr_base_unsigned_dprintf
			(va_arg(*ap, unsigned int), converter, fd);
	else if (converter == 'p')
		count_chars += ft_putnbr_base_unsigned_dprintf
			((uintptr_t)va_arg(*ap, void *), converter, fd);
	else if (converter == '%')
		count_chars += ft_putchar_dprintf('%', fd);
	return (count_chars);
}
