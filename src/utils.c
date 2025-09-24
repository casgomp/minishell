/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 11:40:59 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/24 10:12:58 by rhaas            ###   ########.fr       */
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
		arr = (char **) NULL;
	}
	return (0);
}