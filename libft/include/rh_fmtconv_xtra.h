/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rh_fmtconv_xtra.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 11:29:12 by rhaas             #+#    #+#             */
/*   Updated: 2025/06/08 15:58:36 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RH_FMTCONV_XTRA_H
# define RH_FMTCONV_XTRA_H

# include "rh_fmtspec.h"

int	rh_convspec_invalidfs(const t_fmtspec *const fmtspec);
int	rh_convspec_percent(void);

#endif // RH_FMTCONV_XTRA_H