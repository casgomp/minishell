/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rh_fmtconv_std.h                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 13:58:31 by rhaas             #+#    #+#             */
/*   Updated: 2025/06/06 16:45:47 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RH_FMTCONV_STD_H
# define RH_FMTCONV_STD_H

# include "rh_fmtspec.h"

# include <stdarg.h>

int	rh_convspec_c(va_list *valst, const t_fmtspec *const fmtspec);
int	rh_convspec_s(va_list *valst, const t_fmtspec *const fmtspec);
int	rh_convspec_p(va_list *valst, t_fmtspec *const fmtspec);
int	rh_convspec_diu(va_list *valst, const t_fmtspec *const fmtspec);
int	rh_convspec_x(va_list *valst, t_fmtspec *const fmtspec);

#endif //RH_FMTCONV_STD_H
