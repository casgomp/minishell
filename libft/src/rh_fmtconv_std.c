/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rh_fmtconv_std.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 16:57:50 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/18 11:32:05 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "rh_printf_intern.h"

#include <stdlib.h>

int	rh_convspec_c(va_list *valst, const t_fmtspec *const fmtspec)
{
	const char	c = va_arg((*valst), int);

	return (rh_write_outstr(&c, fmtspec, '\0'));
}

int	rh_convspec_s(va_list *valst, const t_fmtspec *const fmtspec)
{
	const char	*s;

	s = va_arg((*valst), char *);
	if (!s)
	{
		s = "";
		if (-1 == fmtspec->precision || 6 <= fmtspec->precision)
			s = "(null)";
	}
	return (rh_write_outstr(s, fmtspec, '\0'));
}

int	rh_convspec_p(va_list *valst, t_fmtspec *const fmtspec)
{
	const intptr_t	ptrval = va_arg((*valst), intptr_t);
	int				outlen;
	char			*s;

	if (!ptrval)
	{
		s = "(nil)";
		fmtspec->fsflags &= ~((t_fsflag)(altfmt | spacesign | forcesign));
		fmtspec->precision = 0;
	}
	else
	{
		s = ft_ptrtoa_base(ptrval, "0123456789abcdef", 0);
		if (!s)
			return (-1);
	}
	outlen = rh_write_outstr(s, fmtspec, rh_signchar(1, fmtspec));
	if (ptrval)
		free(s);
	return (outlen);
}

int	rh_convspec_diu(va_list *valst, const t_fmtspec *const fmtspec)
{
	const int	n = va_arg((*valst), int);
	size_t		outlen;
	char		*s;
	const char	signc = rh_signchar(n, fmtspec);

	if ((0 == n) && (0 == fmtspec->precision))
		return (rh_write_outstr("", fmtspec, signc));
	s = ft_itoa_base(n, "0123456789", (fmtspec->convspec != 'u'));
	if (!s)
		return (-1);
	outlen = rh_write_outstr(s + (signc == '-'), fmtspec, signc);
	free(s);
	return (outlen);
}

int	rh_convspec_x(va_list *valst, t_fmtspec *const fmtspec)
{
	const unsigned int	ui = va_arg((*valst), unsigned int);
	const char			*base;
	char				*s;
	int					outlen;

	if ((0 == ui))
	{
		fmtspec->fsflags &= ~((t_fsflag)(altfmt));
		if (0 == fmtspec->precision)
			return (rh_write_outstr("", fmtspec, '\0'));
	}
	if ('X' == fmtspec->convspec)
		base = "0123456789ABCDEF";
	else
		base = "0123456789abcdef";
	s = ft_itoa_base(ui, base, 0);
	if (!s)
		return (-1);
	outlen = rh_write_outstr(s, fmtspec, '\0');
	free(s);
	return (outlen);
}
