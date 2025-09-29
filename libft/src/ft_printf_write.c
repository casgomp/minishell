/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_write.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 15:26:54 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/18 19:54:58 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_intern.h"

#include "unistd.h"

int	bytecount_eval(t_bytecount_fd *const bcp)
{
	if (0 <= bcp->lastwrite)
	{
		bcp->total += bcp->lastwrite;
		return (1);
	}
	if (0 < bcp->total)
		bcp->total *= -1;
	else if (0 == bcp->total)
		bcp->total = -1;
	return (0);
}

int	try_write(
	t_bytecount_fd *const pbc,
	const char *const str,
	const size_t len)
{
	pbc->lastwrite = write(pbc->fd, str, len);
	if (!bytecount_eval(pbc))
		return (0);
	return (1);
}
