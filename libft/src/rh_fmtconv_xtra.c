/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rh_fmtconv_xtra.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 11:34:28 by rhaas             #+#    #+#             */
/*   Updated: 2025/07/02 15:34:50 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rh_fmtconv_xtra.h"

#include "libft.h"
#include "rh_write.h"

#include <stdlib.h>

static void	rh_add_invalidfs_flags(char *invfs, const size_t buffsize,
				const t_fmtspec *const fmtspec);
static int	rh_add_invalidfs_width_prec(char *invfs, const size_t buffsize,
				const t_fmtspec *const fmtspec);
static int	rh_print_invfmtstr(const char **pfmt, int invalid_fmtstr_len);

int	rh_convspec_percent(void)
{
	t_bytecount	bc;

	bc = (t_bytecount){0, 0};
	try_write(&bc, "%", 1);
	return (bc.total);
}

int	rh_convspec_invalidfs(const t_fmtspec *const fmtspec)
{
	const size_t	buffsize = fmtspec->fmtstrlen + 2;
	char *const		invfsorig = ft_calloc(buffsize, sizeof(char));
	char			*invfs;
	int				outlen;

	if (!invfsorig)
		return (-1);
	invfs = invfsorig;
	rh_add_invalidfs_flags(invfs, buffsize, fmtspec);
	if (!rh_add_invalidfs_width_prec(invfs, buffsize, fmtspec))
		return (free(invfsorig), -1);
	outlen = rh_print_invfmtstr((const char **)(&invfs), ft_strlen(invfs));
	return (free(invfsorig), outlen);
}

static void	rh_add_invalidfs_flags(char *invfs, const size_t buffsize,
	const t_fmtspec *const fmtspec)
{
	ft_strlcat(invfs, "%", buffsize);
	if (rh_actflx((t_fsflag)altfmt, fmtspec))
		ft_strlcat(invfs, "#", buffsize);
	if (rh_actflx((t_fsflag)forcesign, fmtspec))
		ft_strlcat(invfs, "+", buffsize);
	if (rh_actflx((t_fsflag)spacesign, fmtspec))
		ft_strlcat(invfs, " ", buffsize);
	if (rh_actflx((t_fsflag)lftjust, fmtspec))
		ft_strlcat(invfs, "-", buffsize);
	if (rh_actflx((t_fsflag)lftpad0, fmtspec))
		ft_strlcat(invfs, "0", buffsize);
}

static int	rh_add_invalidfs_width_prec(char *invfs, const size_t buffsize,
	const t_fmtspec *const fmtspec)
{
	char			*itoa;

	if (0 < fmtspec->fieldwidth)
	{
		itoa = ft_itoa(fmtspec->fieldwidth);
		if (!itoa)
			return (0);
		ft_strlcat(invfs, itoa, buffsize);
		free (itoa);
	}
	if (0 <= fmtspec->precision)
	{
		itoa = ft_itoa(fmtspec->precision);
		if (!itoa)
			return (0);
		ft_strlcat(invfs, ".", buffsize);
		ft_strlcat(invfs, itoa, buffsize);
		free (itoa);
	}
	return (1);
}

static int	rh_print_invfmtstr(const char **pfmt, int invalid_fmtstr_len)
{
	t_bytecount			bc;

	bc = (t_bytecount){0, 0};
	try_write(&bc, (*pfmt), invalid_fmtstr_len);
	(*pfmt) += invalid_fmtstr_len;
	return (bc.total);
}
