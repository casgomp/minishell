/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rh_output.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 12:14:12 by rhaas             #+#    #+#             */
/*   Updated: 2025/06/08 12:22:19 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RH_OUTPUT_H
# define RH_OUTPUT_H

# include "rh_fmtspec.h"

# include <stddef.h>

typedef struct s_outstr_fmtspec
{
	size_t	strlen;
	size_t	fieldwidth;
	int		zeropadsize;
	char	*hex;
	char	signc;
}				t_outstr_fmtspec;

int		rh_write_outstr(
			const char *const str,
			const t_fmtspec *const fmtspec,
			const char signc);
int		padding_left(
			const t_fmtspec *const fmtspec,
			t_outstr_fmtspec *poutfs);
int		padding_right(const int outlen_sofar, const t_fmtspec *const fmtspec);
char	rh_signchar(const int n, const t_fmtspec *const fmtspec);

#endif //RH_OUTPUT_H
