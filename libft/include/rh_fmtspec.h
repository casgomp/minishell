/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rh_fmtspec.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 13:51:57 by rhaas             #+#    #+#             */
/*   Updated: 2025/06/06 12:35:43 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RH_FMTSPEC_H
# define RH_FMTSPEC_H

# include <stddef.h>

typedef enum e_fsflag
{
	lftjust = 0x01,
	forcesign = 0x02,
	spacesign = 0x04,
	altfmt = 0x08,
	lftpad0 = 0x10,
}	t_fsflag;

typedef struct s_fmtspec
{
	int		fsflags;
	size_t	fieldwidth;
	int		precision;
	char	convspec;
	int		is_valid;
	int		fmtstrlen;
}			t_fmtspec;

int		rh_set_formatspec(const char **pfmt, t_fmtspec *fmtspec);
int		rh_actflx(t_fsflag flag, const t_fmtspec *const fmtspec);

#endif // RH_FMTSPEC_H
