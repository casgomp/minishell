/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 12:55:46 by raphha            #+#    #+#             */
/*   Updated: 2025/10/20 10:31:49 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

#include "libft.h"
#include "ft_printf_intern.h"

#include <stdarg.h>
#include <stdlib.h>

int	ft_printf(const char *fmt, ...)
{
	va_list			args;
	int				retval;
	t_pfod const	pfod = (t_pfod){.fd = 1, .buf = (char *) NULL};

	va_start(args, fmt);
	retval = ft_coreprintf(pfod, fmt, args);
	va_end(args);
	return (retval);
}

int	ft_dprintf(const int fd, const char *fmt, ...)
{
	va_list			args;
	int				retval;
	t_pfod const	pfod = (t_pfod){.fd = fd, .buf = (char *) NULL};

	va_start(args, fmt);
	retval = ft_coreprintf(pfod, fmt, args);
	va_end(args);
	return (retval);
}

int	ft_sprintf(char *const buf, const char *fmt, ...)
{
	va_list			args;
	int				retval;
	t_pfod const	pfod = (t_pfod){.fd = -1, .buf = buf};

	va_start(args, fmt);
	retval = ft_coreprintf(pfod, fmt, args);
	va_end(args);
	return (retval);
}
