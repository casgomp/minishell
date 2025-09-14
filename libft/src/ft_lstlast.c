/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pecastro <pecastro@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 11:56:25 by pecastro          #+#    #+#             */
/*   Updated: 2025/07/24 11:56:28 by pecastro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_linked_list.h"

t_list	*ft_lstlast(t_list *lst)
{
	t_list	*current;

	if (lst == NULL)
	{
		return (NULL);
	}
	current = lst;
	while (current->next != NULL)
	{
		current = current->next;
	}
	return (current);
}
