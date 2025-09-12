/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 09:26:29 by rhaas             #+#    #+#             */
/*   Updated: 2025/07/01 22:35:55 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

#include <stdlib.h>

static t_lseg	*rh_get_new_lnseg(t_buff const *const pbuff)
{
	t_lseg	*new;

	new = malloc(sizeof(t_lseg));
	if (new)
	{
		new->charr = malloc(pbuff->buffersize * sizeof(char));
		if (new->charr)
			new->next = ((t_lseg *) NULL);
		else
		{
			free(new);
			new = ((t_lseg *) NULL);
		}
	}
	return (new);
}

int	rh_lstadd_back(t_lseg **lslst, t_lseg **lnseg, t_buff const *const pbuff)
{
	t_lseg	*new;

	if (!(lslst && lnseg))
		return (0);
	new = rh_get_new_lnseg(pbuff);
	if (new)
	{
		if (!(*lslst))
			(*lslst) = new;
		if (*lnseg)
			(*lnseg)->next = new;
		(*lnseg) = new;
		return (1);
	}
	return (0);
}

void	rh_lstclear(t_lseg **lst)
{
	if (!(lst))
		return ;
	if (*lst)
	{
		if ((*lst)->next)
			rh_lstclear(&((*lst)->next));
		if ((*lst)->charr)
			free((*lst)->charr);
		free(*lst);
		(*lst) = (t_lseg *) NULL;
	}
}

void	rh_init_buffer(t_buffers *buffs)
{
	int	idx;

	if (buffs->buffer[0].buffersize > 0)
		return ;
	idx = 0;
	while (idx < MAX_FILEBUFFS)
	{
		buffs->buffer[idx].charr = buffs->charr[idx];
		buffs->buffer[idx].buffersize = BUFFER_SIZE;
		++idx;
	}
	buffs->buffer[idx].charr = &buffs->fallback_buffer;
	buffs->buffer[idx].buffersize = 1;
}
