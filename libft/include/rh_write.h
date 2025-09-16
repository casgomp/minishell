/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rh_write.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 15:27:18 by rhaas             #+#    #+#             */
/*   Updated: 2025/06/04 11:32:56 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RH_WRITE_H
# define RH_WRITE_H

# include "stddef.h"

typedef struct s_bytecount
{
	int	lastwrite;
	int	total;
}				t_bytecount;

int	bytecount_eval(t_bytecount *const bcp);
int	try_write(t_bytecount *const pbc, const char *const str, const size_t len);

#endif // RH_WRITE_H
