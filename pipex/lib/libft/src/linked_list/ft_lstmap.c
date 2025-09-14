/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pecastro <pecastro@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 11:55:41 by pecastro          #+#    #+#             */
/*   Updated: 2025/07/24 11:55:43 by pecastro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_linked_list.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	void	*content_m;
	t_list	*new_node;
	t_list	*new_head;

	new_head = NULL;
	while (lst != NULL)
	{
		content_m = f(lst->content);
		if (!content_m)
		{
			ft_lstclear(&new_head, del);
			return (NULL);
		}
		new_node = ft_lstnew(content_m);
		if (new_node == NULL)
		{
			del(content_m);
			ft_lstclear(&new_head, del);
			return (NULL);
		}
		ft_lstadd_back(&new_head, new_node);
		lst = lst->next;
	}
	return (new_head);
}
