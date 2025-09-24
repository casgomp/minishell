/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 11:40:59 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/24 16:12:11 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

int	ms_free_nullterm_array(void **arr)
{
	void	**elem;

	if (arr)
	{
		elem = arr;
		while (*elem)
		{
			free(*elem);
			*elem = (void *) NULL;
			++elem;
		}
		free(arr);
		arr = (void **) NULL;
	}
	return (0);
}
