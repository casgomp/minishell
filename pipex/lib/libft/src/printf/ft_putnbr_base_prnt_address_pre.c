/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_base_prnt_address_pre.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pecastro <pecastro@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 12:00:51 by pecastro          #+#    #+#             */
/*   Updated: 2025/07/24 12:00:54 by pecastro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"
#include "ft_string.h"

int	ft_putnbr_base_prnt_address_pre(uintptr_t nb, int 
		*count_chars, int prefix, int first_call)
{
	if (count_chars == NULL)
		return (-1);
	if (first_call && nb == 0)
	{
		if (ft_putstr_prnt("(nil)", count_chars) == -1)
			return (-1);
		return (0);
	}
	if (prefix && first_call && nb != 0)
	{
		if (ft_putstr_prnt("0x", count_chars) == -1)
			return (-1);
	}
	if ((size_t)nb >= ft_strlen("0123456789abcdef"))
	{
		ft_putnbr_base_prnt_address_pre(
			nb / ft_strlen("0123456789abcdef"), count_chars, 0, 0);
		nb = nb % ft_strlen("0123456789abcdef");
	}
	if (ft_putchar_prnt("0123456789abcdef"[nb], count_chars) == -1)
		return (-1);
	return (0);
}
