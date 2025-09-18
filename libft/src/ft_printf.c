/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 12:55:46 by raphha            #+#    #+#             */
/*   Updated: 2025/09/18 13:40:35 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

#include "libft.h"
#include "rh_printf_intern.h"

#include <stdarg.h>
#include <stdlib.h>

static int	rh_print_fmtstr(t_bytecount_fd *const pbc, const char **pfmt);
static int	rh_print_arg(t_bytecount_fd *const pbc, const char **pfmt,
				va_list *valst);
int	ft_xprintf(const int fd, const char *fmt, ...);

int	ft_printf(const char *fmt, ...)
{
	va_list	args;
	int		retval;
	
	va_start(args, fmt);
	retval = ft_xprintf(1, fmt, args);
    va_end(args);
	return (retval);
}

int	ft_xprintf(const int fd, const char *fmt, ...)
{
	va_list		valst;
	t_bytecount_fd	bc;

	bc = (t_bytecount_fd){.lastwrite=0, .total=0, .fd=fd};
	va_start(valst, fmt);
	while (*fmt)
	{
		if (!rh_print_fmtstr(&bc, &fmt))
			return (bc.total);
		if ((*fmt) && (*fmt) == '%')
			if (!rh_print_arg(&bc, &fmt, &valst))
				return (bc.total);
	}
	va_end(valst);
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
				va_list *valst)
{
	t_fmtspec	fmtspec;

	rh_set_formatspec(pfmt, &fmtspec);
	if (!fmtspec.is_valid)
		/* pbc->lastwrite = ( */rh_convspec_invalidfs(&fmtspec, pbc)/* ) */;
	else if (fmtspec.convspec == 'c')
		/* pbc->lastwrite = ( */rh_convspec_c(valst, &fmtspec, pbc)/* ) */;
	else if (fmtspec.convspec == 's')
		pbc->lastwrite = (rh_convspec_s(valst, &fmtspec, pbc));
	else if (fmtspec.convspec == 'p')
		pbc->lastwrite = (rh_convspec_p(valst, &fmtspec, pbc));
	else if (ft_strchr("diu", fmtspec.convspec))
		pbc->lastwrite = (rh_convspec_diu(valst, &fmtspec, pbc));
	else if (ft_tolower(fmtspec.convspec) == 'x')
		pbc->lastwrite = (rh_convspec_x(valst, &fmtspec, pbc));
	else if (fmtspec.convspec == '%')
		/* bcp->lastwrite = ( */rh_convspec_percent(pbc)/* ) */;
	// bytecount_eval(bcp);
	return (0 <= pbc->total);
}
