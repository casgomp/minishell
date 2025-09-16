/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 16:13:30 by rhaas             #+#    #+#             */
/*   Updated: 2025/05/21 21:13:55 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	int	llen;

	llen = 0;
	if (lst)
	{
		llen = 1;
		while (lst->next)
		{
			++llen;
			lst = lst->next;
		}
	}
	return (llen);
}
