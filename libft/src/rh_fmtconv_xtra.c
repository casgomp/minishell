/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rh_fmtconv_xtra.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 11:34:28 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/18 13:40:35 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "rh_printf_intern.h"

#include <stdlib.h>

static void	rh_add_invalidfs_flags(char *invfs, const size_t buffsize,
				const t_fmtspec *const fmtspec);
static int	rh_add_invalidfs_width_prec(char *invfs, const size_t buffsize,
				const t_fmtspec *const fmtspec);
static int	rh_print_invfmtstr(const char **pfmt, int invalid_fmtstr_len,
				t_bytecount_fd *const pbc);

int	rh_convspec_percent(t_bytecount_fd *const pbc)
{
	// t_bytecount_fd	bc;

	// *pbc = (t_bytecount_fd){0, 0};
	// try_write(&bc, "%", 1);
	try_write(pbc, "%", 1);
	return (1);
}

int	rh_convspec_invalidfs(
	const t_fmtspec *const fmtspec, t_bytecount_fd *const pbc)
{
	const size_t	buffsize = fmtspec->fmtstrlen + 2;
	char *const		invfsorig = ft_calloc(buffsize, sizeof(char));
	char			*invfs;

	if (!invfsorig)
		return (0);
	invfs = invfsorig;
	rh_add_invalidfs_flags(invfs, buffsize, fmtspec);
	if (!rh_add_invalidfs_width_prec(invfs, buffsize, fmtspec))
		return (free(invfsorig), 0);
	rh_print_invfmtstr((const char **)(&invfs), ft_strlen(invfs), pbc);
	return (free(invfsorig), 1);
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

static int	rh_print_invfmtstr(
	const char **pfmt,
	int invalid_fmtstr_len,
	t_bytecount_fd *const pbc)
{
	// t_bytecount_fd			bc;

	// bc = (t_bytecount_fd){0, 0};
	try_write(pbc, (*pfmt), invalid_fmtstr_len);
	(*pfmt) += invalid_fmtstr_len;
	return (1);
}
