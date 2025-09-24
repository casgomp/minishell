/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 11:40:59 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/24 14:26:31 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

int	ms_free_nullterm_array(char **arr)
{
	char	**substr;

	if (arr)
	{
		substr = arr;
		while (*substr)
		{
			free(*(substr++));
		}
		free(arr);
		arr = (char **) NULL;
	}
	return (0);
}