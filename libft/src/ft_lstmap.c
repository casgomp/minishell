/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 16:56:14 by rhaas             #+#    #+#             */
/*   Updated: 2025/05/21 21:16:20 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list *const	origin = lst;
	t_list			*new_list;
	t_list			*lst_elem;
	t_list			*lst_prev_elem;

	new_list = (t_list *) NULL;
	while (lst)
	{
		lst_elem = malloc(sizeof(t_list));
		if (lst_elem)
		{
			lst_elem->content = (*f)(lst->content);
			lst_elem->next = (t_list *) NULL;
			if (lst == origin)
				new_list = lst_elem;
			else
				lst_prev_elem->next = lst_elem;
			lst_prev_elem = lst_elem;
		}
		else
			return (ft_lstclear(&new_list, del), (t_list *) NULL);
		lst = lst->next;
	}
	return (new_list);
}
