/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dprintf.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pecastro <pecastro@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 11:53:01 by pecastro          #+#    #+#             */
/*   Updated: 2025/08/19 11:10:36 by pecastro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef FT_DPRINTF_H
# define FT_DPRINTF_H

# include <stdarg.h>
# include <stdio.h>
# include <stdint.h>
# include <stdlib.h>
# include <unistd.h>

int	ft_dprintf(int fd, const char *format, ...);
int	ft_putchar_dprintf(char c, int fd);
int	ft_putstr_dprintf(char *s, int fd);
int	ft_putnbr_dprintf(int nb, int fd);
int	ft_putnbr_base_unsigned_dprintf(unsigned long long nb, 
		char converter, int fd);

#endif
