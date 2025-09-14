/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_string.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pecastro <pecastro@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 11:52:16 by pecastro          #+#    #+#             */
/*   Updated: 2025/08/20 11:40:44 by pecastro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef FT_STRING_H
# define FT_STRING_H

# include <stddef.h>
# include <limits.h>

char			*ft_strchr(const char *s, int c);
char			*ft_strrchr(const char *s, int c);
char			*ft_strnstr(const char *big, const char *little, size_t len);
int				ft_strcmp(const char *s1, const char *s2);
int				ft_strncmp(const char *s1, const char *s2, size_t n);
int				ft_atoi(const char *nptr);
//ft_atoi safe
int				ft_atoi_safe(const char *nptr, int *error);
unsigned int	ft_atoi_safe_digit(const char *nptr, 
					int i, int *error, int sign);
//
size_t			ft_strlcat(char *dst, const char *src, size_t size);
size_t			ft_strlcpy(char *dst, const char *src, size_t size);
size_t			ft_strlen(const char *s);

#endif
