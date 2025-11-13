/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rh_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 17:00:02 by rhaas             #+#    #+#             */
/*   Updated: 2025/11/13 11:32:24 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

void	rh_free_simple_array(void ***arr)
{
	void	**elem;

	if (*arr)
	{
		elem = *arr;
		while (*elem)
			free(*(elem++));
		free(*arr);
		*arr = (void **) NULL;
	}
}
// // Doesn't work in C, because ->next is not accepted on a nullptr
// // ..but can still be used as a template
// void	rh_free_simple_list(void **lst_head)
// {
// 	void	*elem;

// 	while (*lst_head)
// 	{
// 		elem = (*lst_head)->next;
// 		free(*lst_head);
// 		(*lst_head) = elem;
// 	}
// 	(*lst_head) = (void *) NULL;
// }
