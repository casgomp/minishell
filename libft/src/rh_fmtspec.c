/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rh_fmtspec.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 16:46:18 by rhaas             #+#    #+#             */
/*   Updated: 2025/06/06 19:06:58 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "rh_printf_intern.h"

static void	rh_init_formatspec(t_fmtspec *fmtspec);
static void	rh_set_formatflags(const char c, t_fmtspec *fmtspec);
static void	rh_prune_fmtflags(t_fmtspec *fmtspec);

int	rh_set_formatspec(const char **pfmt, t_fmtspec *fmtspec)
{
	const char *const	startpos = *pfmt;

	rh_init_formatspec(fmtspec);
	while (*(++(*pfmt)) && ft_strchr("+- #0", **pfmt))
		rh_set_formatflags(**pfmt, fmtspec);
	--(*pfmt);
	while (*(++(*pfmt)) && ft_isdigit(**pfmt))
		fmtspec->fieldwidth = 10 * fmtspec->fieldwidth + (int)(**pfmt - '0');
	if ((**pfmt) && (**pfmt == '.'))
	{
		fmtspec->precision = 0;
		while (*(++(*pfmt)) && ft_isdigit(**pfmt))
			fmtspec->precision = 10 * fmtspec->precision + (int)(**pfmt - '0');
	}
	--(*pfmt);
	if (*(++(*pfmt)) && ft_strchr("cspdiuxX%", **pfmt))
		fmtspec->convspec = *((*pfmt));
	else
		fmtspec->is_valid = 0;
	(*pfmt) += fmtspec->is_valid;
	fmtspec->fmtstrlen = (int)((*pfmt) - startpos);
	rh_prune_fmtflags(fmtspec);
	return (fmtspec->fmtstrlen);
}

int	rh_actflx(t_fsflag flag_s, const t_fmtspec *const fmtspec)
{
	return ((flag_s & fmtspec->fsflags) == flag_s);
}

static void	rh_init_formatspec(t_fmtspec *fmtspec)
{
	fmtspec->fsflags = 0;
	fmtspec->fieldwidth = 0;
	fmtspec->precision = -1;
	fmtspec->convspec = '\0';
	fmtspec->fmtstrlen = 0;
	fmtspec->is_valid = 1;
}

static void	rh_set_formatflags(const char c, t_fmtspec *fmtspec)
{
	if ('+' == c)
		(fmtspec->fsflags |= (t_fsflag)forcesign);
	if ('-' == c)
		(fmtspec->fsflags |= (t_fsflag)lftjust);
	if (' ' == c)
		(fmtspec->fsflags |= (t_fsflag)spacesign);
	if ('#' == c)
		fmtspec->fsflags |= (t_fsflag)altfmt;
	if ('0' == c)
		fmtspec->fsflags |= (t_fsflag)lftpad0;
}

static void	rh_prune_fmtflags(t_fmtspec *fmtspec)
{
	if ((fmtspec->convspec == 's') || (fmtspec->convspec == 'c'))
		fmtspec->fsflags &= ~((t_fsflag)(lftpad0));
	if (fmtspec->convspec == 'p')
		fmtspec->fsflags |= (t_fsflag)(altfmt);
	if ((fmtspec->convspec != '\0') && ft_strchr("uxX", fmtspec->convspec))
		fmtspec->fsflags &= ~((t_fsflag)(forcesign | spacesign));
	if (rh_actflx((t_fsflag)lftjust, fmtspec))
		fmtspec->fsflags &= ~((t_fsflag)(lftpad0));
	if (rh_actflx((t_fsflag)forcesign, fmtspec))
		fmtspec->fsflags &= ~((t_fsflag)(spacesign));
}
