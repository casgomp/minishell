/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pecastro <pecastro@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 12:06:52 by pecastro          #+#    #+#             */
/*   Updated: 2025/07/24 12:06:55 by pecastro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_fd.h"
#include "ft_string.h"

void	ft_putstr_fd(char *s, int fd)
{
	write(fd, s, ft_strlen((const char *)(s)));
}
