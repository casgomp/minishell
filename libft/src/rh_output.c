/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rh_output.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 12:13:58 by rhaas             #+#    #+#             */
/*   Updated: 2025/06/06 19:07:51 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rh_output.h"

#include "libft.h"
#include "rh_write.h"

#include <limits.h>

static void	rh_set_outstr_fmtspec(
	t_outstr_fmtspec *poutfmtspc,
	const t_fmtspec *const pfmtspec,
	const char signc)
{
	poutfmtspc->fieldwidth = pfmtspec->fieldwidth;
	poutfmtspc->hex = (char *) NULL;
	poutfmtspc->signc = signc;
	if (ft_strchr("px", pfmtspec->convspec)
		&& rh_actflx(altfmt, pfmtspec))
		poutfmtspc->hex = "0x";
	else if ((pfmtspec->convspec == 'X')
		&& rh_actflx(altfmt, pfmtspec))
		poutfmtspc->hex = "0X";
	poutfmtspc->zeropadsize = 0;
	if (0 <= pfmtspec->precision)
	{
		if ((pfmtspec->convspec == 's')
			&& (poutfmtspc->strlen > (size_t)pfmtspec->precision))
			poutfmtspc->strlen = pfmtspec->precision;
		else if ((ft_strchr("pdiuxX", pfmtspec->convspec))
			&& (poutfmtspc->strlen < (size_t)pfmtspec->precision))
			poutfmtspc->zeropadsize = pfmtspec->precision - poutfmtspc->strlen;
	}
	else if (rh_actflx(lftpad0, pfmtspec) && (!rh_actflx(lftjust, pfmtspec)))
		poutfmtspc->zeropadsize = pfmtspec->fieldwidth - (poutfmtspc->strlen
				+ (poutfmtspc->signc != 0) + 2 * (poutfmtspc->hex != NULL));
	if (poutfmtspc->zeropadsize < 0)
		poutfmtspc->zeropadsize = 0;
}

int	rh_write_outstr(
				const char *const str,
				const t_fmtspec *const fmtspec,
				const char signc)
{
	t_outstr_fmtspec	outfmtspec;
	t_bytecount			bc;

	bc = (t_bytecount){0, 0};
	if (fmtspec->convspec == 'c')
		outfmtspec.strlen = 1;
	else
		outfmtspec.strlen = ft_strlen(str);
	if (outfmtspec.strlen > (size_t)INT_MAX)
		return (-1);
	rh_set_outstr_fmtspec(&outfmtspec, fmtspec, signc);
	bc.lastwrite = padding_left(fmtspec, &outfmtspec);
	if (!bytecount_eval(&bc))
		return (bc.total);
	if (!try_write(&bc, str, outfmtspec.strlen))
		return (bc.total);
	bc.lastwrite = padding_right(bc.total, fmtspec);
	bytecount_eval(&bc);
	return (bc.total);
}

int	padding_left(
	const t_fmtspec *const pfmtspec,
	t_outstr_fmtspec *poutfs)
{
	t_bytecount			bc;

	bc = (t_bytecount){0, 0};
	if (!rh_actflx(lftjust, pfmtspec))
	{
		while ((poutfs->fieldwidth--) > (poutfs->strlen + (poutfs->signc != 0)
				+ 2 * (poutfs->hex != NULL) + poutfs->zeropadsize))
		{
			if (!try_write(&bc, " ", 1))
				return (bc.total);
		}
	}
	if (!try_write(&bc, &poutfs->signc, (poutfs->signc != 0)))
		return (bc.total);
	if (!try_write(&bc, poutfs->hex, 2 * (poutfs->hex != (char *) NULL)))
		return (bc.total);
	while (poutfs->zeropadsize--)
		if (!try_write(&bc, "0", 1))
			return (bc.total);
	return (bc.total);
}

int	padding_right(const int outlen_sofar, const t_fmtspec *const pfmtspec)
{
	int			remaining_fieldwidth;
	t_bytecount	bc;

	bc = (t_bytecount){0, 0};
	remaining_fieldwidth = pfmtspec->fieldwidth - outlen_sofar;
	while (remaining_fieldwidth-- > 0)
	{
		if (!try_write(&bc, " ", 1))
			return (bc.total);
	}
	return (bc.total);
}

char	rh_signchar(const int n, const t_fmtspec *const pfmtspec)
{
	char	signc;

	signc = '\0';
	if ((pfmtspec->convspec == 'u') || (pfmtspec->convspec == 'p') || (0 <= n))
	{
		if (rh_actflx(forcesign, pfmtspec))
			signc = '+';
		else if (rh_actflx(spacesign, pfmtspec))
			signc = ' ';
	}
	else if (0 > n)
		signc = '-';
	return (signc);
}
