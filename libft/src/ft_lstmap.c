/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 16:56:14 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/22 13:04:17 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_list;
	t_list	*lst_elem;

	if (!(lst && f && del))
		return ((t_list *) NULL);
	new_list = (t_list *) NULL;
	while (lst)
	{
		lst_elem = ft_lstnew((*f)(lst->content));
		if (lst_elem)
			ft_lstadd_back(&new_list, lst_elem);
		else
			return (ft_lstclear(&new_list, del), (t_list *) NULL);
		lst = lst->next;
	}
	return (new_list);
}
