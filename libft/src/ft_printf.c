/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 12:55:46 by raphha            #+#    #+#             */
/*   Updated: 2025/09/18 11:31:24 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

#include "libft.h"
#include "rh_printf_intern.h"

#include <stdarg.h>
#include <stdlib.h>

static int	rh_print_fmtstr(t_bytecount *const bcp, const char **pfmt);
static int	rh_print_arg(t_bytecount *const bcp, const char **pfmt,
				va_list *valst);

int	ft_printf(const char *fmt, ...)
{
	va_list		valst;
	t_bytecount	bc;

	bc = (t_bytecount){0, 0};
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

static int	rh_print_fmtstr(t_bytecount *const bcp, const char **pfmt)
{
	const char *const	startpos = *pfmt;
	size_t				outstrlen;

	while ((**pfmt) && (**pfmt) != '%')
		++(*pfmt);
	outstrlen = (*pfmt) - startpos;
	return (try_write(bcp, startpos, outstrlen));
}

static int	rh_print_arg(t_bytecount *const bcp, const char **pfmt,
				va_list *valst)
{
	t_fmtspec	fmtspec;

	rh_set_formatspec(pfmt, &fmtspec);
	if (!fmtspec.is_valid)
		bcp->lastwrite = (rh_convspec_invalidfs(&fmtspec));
	else if (fmtspec.convspec == 'c')
		bcp->lastwrite = (rh_convspec_c(valst, &fmtspec));
	else if (fmtspec.convspec == 's')
		bcp->lastwrite = (rh_convspec_s(valst, &fmtspec));
	else if (fmtspec.convspec == 'p')
		bcp->lastwrite = (rh_convspec_p(valst, &fmtspec));
	else if (ft_strchr("diu", fmtspec.convspec))
		bcp->lastwrite = (rh_convspec_diu(valst, &fmtspec));
	else if (ft_tolower(fmtspec.convspec) == 'x')
		bcp->lastwrite = (rh_convspec_x(valst, &fmtspec));
	else if (fmtspec.convspec == '%')
		bcp->lastwrite = (rh_convspec_percent());
	bytecount_eval(bcp);
	return (0 <= bcp->total);
}
