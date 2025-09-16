/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 16:31:25 by rhaas             #+#    #+#             */
/*   Updated: 2025/05/21 21:15:33 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stddef.h>

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	if (!(del && lst))
		return ;
	if (*lst)
	{
		if ((*lst)->next)
			ft_lstclear(&((*lst)->next), del);
		ft_lstdelone((*lst), del);
		(*lst) = (t_list *) NULL;
	}
}
