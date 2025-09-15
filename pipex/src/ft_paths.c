/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_paths.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pecastro <pecastro@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 11:48:10 by pecastro          #+#    #+#             */
/*   Updated: 2025/08/31 12:05:19 by pecastro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "pipex.h"

char	**ft_complete_paths(char *cmd)
{
	char	**path_raw_arr;
	char	**paths;
	int		path_count;
	int		i;

	path_raw_arr = ft_paths_from_environ();
	if (!path_raw_arr)
		return (NULL);
	path_count = ft_size_arr(path_raw_arr);
	paths = malloc(sizeof(char *) * (path_count + 1));
	if (!paths)
		return (ft_clean_arr(path_raw_arr), NULL);
	i = 0;
	while (path_raw_arr[i])
	{
		paths[i] = ft_strjoin(path_raw_arr[i], cmd);
		if (!paths[i])
			return (ft_clean_arr_partial(i, paths), NULL);
		i ++;
	}
	paths[i] = NULL;
	return (ft_clean_arr(path_raw_arr), paths);
}

char	**ft_paths_from_environ(void)
{
	int		i;
	char	*search_word;

	search_word = "PATH=";
	i = 0;
	while (environ[i])
	{
		if (ft_strncmp(environ[i], search_word, ft_strlen(search_word)) == 0)
			return (ft_split(environ[i] + 5, ':'));
		i ++;
	}
	return (NULL);
}

int	ft_access_paths(t_pipex *pipex)
{
	int	i;
	int	j;
	int	flag[2];

	flag[0] = 1;
	flag[1] = 1;
	i = 0;
	while (i < 2)
	{
		j = 0;
		while (pipex->complete_paths[i][j])
		{
			if (access(pipex->complete_paths[i][j], X_OK) == 0)
			{
				pipex->path[i] = pipex->complete_paths[i][j];
				flag[i] = 0;
				break ;
			}
			j ++;
		}
		if (flag[i])
			perror("Error: access()");
		i ++;
	}
	return (flag[0] + flag[1]);
}

char	ft_contains_words(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] != ' ')
			return (str[i]);
		i ++;
	}
	return ('\0');
}

int	ft_size_arr(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		i ++;
	return (i);
}
