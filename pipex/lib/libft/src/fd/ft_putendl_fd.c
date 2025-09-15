/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pecastro <pecastro@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 12:06:35 by pecastro          #+#    #+#             */
/*   Updated: 2025/07/24 12:06:37 by pecastro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_fd.h"
#include "ft_string.h"

void	ft_putendl_fd(char *s, int fd)
{
	write(fd, s, ft_strlen((const char *)s));
	write(fd, "\n", 1);
}
