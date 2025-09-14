/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_prnt.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pecastro <pecastro@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 12:00:35 by pecastro          #+#    #+#             */
/*   Updated: 2025/07/24 12:00:38 by pecastro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"

int	ft_putchar_prnt(int c, int *count_chars)
{
	if (write(1, &c, 1) == -1)
		return (-1);
	(*count_chars)++;
	return (0);
}
