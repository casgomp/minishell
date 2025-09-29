/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rh_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 17:00:02 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/24 17:00:59 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

int	rh_free_simple_array(void **arr)
{
	void	**elem;

	if (arr)
	{
		elem = arr;
		while (*elem)
			free(*(elem++));
		free(arr);
		arr = (void **) NULL;
	}
	return (1);
}
