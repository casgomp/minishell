/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_dprintf.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pecastro <pecastro@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 13:26:11 by pecastro          #+#    #+#             */
/*   Updated: 2025/08/09 16:18:17 by pecastro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_dprintf.h"

int	ft_putchar_dprintf(char c, int fd)
{
	return (write(fd, &c, 1));
}
