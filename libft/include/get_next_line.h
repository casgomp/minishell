/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 09:27:07 by rhaas             #+#    #+#             */
/*   Updated: 2025/07/01 22:45:42 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <sys/types.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 512
//# elif 0 == BUFFER_SIZE
//#  undef BUFFER_SIZE
//#  define BUFFER_SIZE 1
# endif
# define MAX_FILEBUFFS 32

typedef struct s_buff
{
	int		fd;
	ssize_t	bytes_read;
	ssize_t	idx;
	ssize_t	buffersize;
	char	*charr;
}	t_buff;

typedef struct s_buffers
{
	char	charr[MAX_FILEBUFFS][BUFFER_SIZE];
	t_buff	buffer[MAX_FILEBUFFS + 1];
	char	fallback_buffer;
}	t_buffers;

typedef struct s_linesegment
{
	char					*charr;
	struct s_linesegment	*next;
}	t_lseg;

typedef struct s_line
{
	char	*str;
	ssize_t	len;
	ssize_t	eolat;
	t_lseg	*lnseglist;
	t_lseg	*lnseg;
}	t_line;

char	*get_next_line(int fd);
void	rh_init_buffer(t_buffers *buffs);
int		rh_lstadd_back(t_lseg **lslst, t_lseg **lnseg,
			t_buff const *const pbuff);
void	rh_lstclear(t_lseg **lst);

#endif // GET_NEXT_LINE_H