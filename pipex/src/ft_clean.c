/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_clean.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pecastro <pecastro@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 14:19:50 by pecastro          #+#    #+#             */
/*   Updated: 2025/08/31 12:29:07 by pecastro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "pipex.h"

void	ft_clean_arr(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i ++;
	}
	free(arr);
}

void	ft_clean_arr_partial(int index, char **arr)
{
	int	i;

	i = index - 1;
	while (i >= 0)
	{
		free (arr[i]);
		i --;
	}
	free (arr);
}

void	ft_free(int index, t_pipex *pipex)
{
	int	i;

	i = 0;
	while (i <= index)
	{
		ft_clean_arr(pipex->cmd[i]);
		free(pipex->cmd_rel[i]);
		ft_clean_arr(pipex->complete_paths[i]);
		i ++;
	}
}
