/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pecastro <pecastro@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 11:59:57 by pecastro          #+#    #+#             */
/*   Updated: 2025/07/24 12:00:00 by pecastro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"

static int	ft_is_converter(char c)
{
	size_t	i;
	char	*str;

	str = "cspdiuxX";
	i = 0;
	while (str[i])
	{
		if (str[i] == c)
		{
			return (1);
		}
		i ++;
	}
	return (0);
}

static void	ft_convert(char converter, va_list *ap, int *count_chars)
{
	int	flag;

	if (converter == 'c')
		flag = ft_putchar_prnt(va_arg(*ap, int), count_chars);
	else if (converter == 's')
		flag = ft_putstr_prnt(va_arg(*ap, char *), count_chars);
	else if (converter == 'p')
		flag = ft_putnbr_base_prnt_address_pre((uintptr_t)va_arg(*ap, void *), 
				count_chars, 1, 1);
	else if (converter == 'd' || converter == 'i')
		flag = ft_putnbr_base_prnt(va_arg(*ap, int), count_chars, "0123456789");
	else if (converter == 'u')
		flag = ft_putnbr_base_prnt_address(va_arg(*ap, unsigned int), 
				count_chars, "0123456789");
	else if (converter == 'x')
		flag = ft_putnbr_base_prnt_address(va_arg(*ap, unsigned int), 
				count_chars, "0123456789abcdef");
	else if (converter == 'X')
		flag = ft_putnbr_base_prnt_address(va_arg(*ap, unsigned int),
				count_chars, "0123456789ABCDEF");
	else
		flag = 0;
	if (flag == -1)
		(*count_chars) = -1;
}

int	ft_printf(const char *format, ...)
{
	va_list	ap;
	int		count_chars;
	size_t	i;

	if (format == NULL)
		return ('\0');
	va_start(ap, format);
	count_chars = 0;
	i = 0;
	while (format[i])
	{
		if (format[i] == '%' && ft_is_converter(format[i + 1]) == 1)
		{
			ft_convert(format[i + 1], &ap, &count_chars);
			if (count_chars == -1)
				return (count_chars);
			i += 2;
		}
		if (format[i] == '%' && format[i + 1] == '%')
			i ++;
		if (!(format[i] == '\0'))
			ft_putchar_prnt(format[i++], &count_chars);
	}
	va_end(ap);
	return (count_chars);
}
