/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 12:55:46 by raphha            #+#    #+#             */
/*   Updated: 2025/10/16 11:35:09 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

#include "libft.h"
#include "ft_printf_intern.h"

#include <stdarg.h>
#include <stdlib.h>

static int	rh_print_fmtstr(t_bytecount_fd *const pbc, const char **pfmt);
static int	rh_print_arg(t_bytecount_fd *const pbc, const char **pfmt,
				va_list valst);
static int	ft_coreprintf(const t_pfod pfod, const char *fmt, va_list valst);

int	ft_printf(const char *fmt, ...)
{
	va_list	args;
	int		retval;

	va_start(args, fmt);
	retval = ft_coreprintf((t_pfod){.fd = 1, .buf = (char *) NULL}, fmt, args);
	va_end(args);
	return (retval);
}

int	ft_dprintf(const int fd, const char *fmt, ...)
{
	va_list	args;
	int		retval;

	va_start(args, fmt);
	retval = ft_coreprintf((t_pfod){.fd = fd, .buf = (char *) NULL}, fmt, args);
	va_end(args);
	return (retval);
}

int	ft_sprintf(char *const buf, const char *fmt, ...)
{
	va_list	args;
	int		retval;

	va_start(args, fmt);
	retval = ft_coreprintf((t_pfod){.fd = -1, .buf = buf}, fmt, args);
	va_end(args);
	return (retval);
}

int	ft_coreprintf(const t_pfod pfod, const char *fmt, va_list valst)
{
	t_bytecount_fd	bc;

	bc = (t_bytecount_fd){.lastwrite = 0, .total = 0, .pfod = pfod};
	while (*fmt)
	{
		if (!rh_print_fmtstr(&bc, &fmt))
			return (bc.total);
		if ((*fmt) && (*fmt) == '%')
			if (!rh_print_arg(&bc, &fmt, valst))
				return (bc.total);
	}
	return (bc.total);
}

static int	rh_print_fmtstr(t_bytecount_fd *const pbc, const char **pfmt)
{
	const char *const	startpos = *pfmt;
	size_t				outstrlen;

	while ((**pfmt) && (**pfmt) != '%')
		++(*pfmt);
	outstrlen = (*pfmt) - startpos;
	return (try_write(pbc, startpos, outstrlen));
}

static int	rh_print_arg(t_bytecount_fd *const pbc, const char **pfmt,
				va_list valst)
{
	t_fmtspec	fmtspec;

	rh_set_formatspec(pfmt, &fmtspec);
	if (!fmtspec.is_valid)
		rh_convspec_invalidfs(&fmtspec, pbc);
	else if (fmtspec.convspec == 'c')
		rh_convspec_c(valst, &fmtspec, pbc);
	else if (fmtspec.convspec == 's')
		(rh_convspec_s(valst, &fmtspec, pbc));
	else if (fmtspec.convspec == 'p')
		(rh_convspec_p(valst, &fmtspec, pbc));
	else if (ft_strchr("diu", fmtspec.convspec))
		(rh_convspec_diu(valst, &fmtspec, pbc));
	else if (ft_tolower(fmtspec.convspec) == 'x')
		(rh_convspec_x(valst, &fmtspec, pbc));
	else if (fmtspec.convspec == '%')
		rh_convspec_percent(pbc);
	return (0 <= pbc->total);
}
