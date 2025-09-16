/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 11:23:49 by rhaas             #+#    #+#             */
/*   Updated: 2025/05/20 14:42:57 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <unistd.h>

void	ft_putnbr_fd(int n, int fd)
{
	unsigned int	ui;
	char			d;

	ui = (unsigned int)n;
	if (n < 0)
	{
		if (n > INT_MIN)
			ui = -n;
		write(fd, "-", 1);
	}
	if (ui >= 10)
		ft_putnbr_fd(ui / 10, fd);
	d = (char)(ui % 10 + '0');
	write(fd, &d, 1);
}
