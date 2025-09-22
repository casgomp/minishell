/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 09:26:04 by rhaas             #+#    #+#             */
/*   Updated: 2025/07/01 23:11:08 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <inttypes.h>
#include <stdlib.h>
#include <unistd.h>

#include "get_next_line.h"

static int	rh_get_buffer(t_buffers *pbuffs, t_buff **pbuff, const int fd)
			;
static int	rh_read_file(t_buff *pbuff, char *buffarr)
			;
static void	rh_compose_linestr(t_line *pline, t_buff *pbuff)
			;
static int	rh_fill_lnseglist(t_line *pline, t_buff *pbuff)
			;

char	*get_next_line(int fd)
{
	static t_buffers	buffers;	
	t_buff				*pbuff;
	t_line				line;

	line = (t_line){(char *) NULL, 0, 0, (t_lseg *) NULL, (t_lseg *) NULL};
	if (rh_get_buffer(&buffers, &pbuff, fd)
		&& (rh_read_file(pbuff, (char *) NULL) > 0)
		&& rh_fill_lnseglist(&line, pbuff))
		rh_compose_linestr(&line, pbuff);
	return (rh_lstclear(&line.lnseglist), line.str);
}

static int	rh_get_buffer(t_buffers *pbuffs, t_buff **pbuff, const int fd)
{
	int					cbuff;
	int					idx;

	rh_init_buffer(pbuffs);
	(*pbuff) = (t_buff *) NULL;
	cbuff = -1;
	idx = 0;
	while (!(*pbuff) && idx < MAX_FILEBUFFS)
	{
		if (fd == pbuffs->buffer[idx].fd)
			(*pbuff) = &(pbuffs->buffer[idx]);
		else if ((cbuff < 0)
			&& (pbuffs->buffer[idx].idx >= pbuffs->buffer[idx].bytes_read))
			cbuff = idx;
		++idx;
	}
	if (!(*pbuff))
	{
		if (cbuff >= 0)
			idx = cbuff;
		(*pbuff) = &(pbuffs->buffer[idx]);
		(*pbuff)->fd = fd;
		(*pbuff)->bytes_read = 0;
	}
	return ((*pbuff)->buffersize == BUFFER_SIZE && (*pbuff) != (t_buff *) NULL);
}

static int	rh_read_file(t_buff *pbuff, char *buffarr)
{
	if (!pbuff)
		return (pbuff->bytes_read = -1, -1);
	if ((!buffarr) && pbuff->idx >= pbuff->bytes_read)
	{
		pbuff->idx = 0;
		buffarr = pbuff->charr;
	}
	if (buffarr)
		pbuff->bytes_read = read(pbuff->fd, buffarr, BUFFER_SIZE);
	return (pbuff->bytes_read);
}

static void	rh_compose_linestr(t_line *pline, t_buff *pbuff)
{
	ssize_t		idx;
	char *const	charr = pbuff->charr;

	if (0 == pline->len)
		return ;
	pline->str = (char *)malloc((pline->len + 1) * sizeof(char));
	if (!pline->str)
		return ;
	pline->str[pline->len] = '\0';
	idx = -1;
	while ((++idx < pline->len) && (pbuff->idx + idx) < pbuff->buffersize)
		pline->str[idx] = charr[pbuff->idx + idx];
	pline->lnseg = pline->lnseglist;
	while (pline->lnseg && idx < pline->len)
	{
		pline->str[idx]
			= pline->lnseg->charr[(pbuff->idx + idx) % pbuff->buffersize];
		if (0 == ((++idx + pbuff->idx) % pbuff->buffersize))
			pline->lnseg = pline->lnseg->next;
	}
	pbuff->idx = pline->eolat;
	if (pline->lnseg && pline->eolat > 0)
		while ((pline->eolat)++ < pbuff->bytes_read)
			charr[pline->eolat - 1]
				= pline->lnseg->charr[pline->eolat - 1];
}

static int	rh_fill_lnseglist(t_line *pline, t_buff *pbuff)
{
	int					idx;
	const char *const	charr = pbuff->charr;

	while ((pbuff->idx + pline->len < pbuff->bytes_read) && (!pline->eolat))
		if ('\n' == charr[pbuff->idx + ((pline->len)++)])
			pline->eolat = pbuff->idx + pline->len;
	while (!pline->eolat)
	{
		if ((!rh_lstadd_back(&pline->lnseglist, &pline->lnseg, pbuff))
			|| (-1 == rh_read_file(pbuff, pline->lnseg->charr)))
			return (0);
		if (0 == pbuff->bytes_read)
			break ;
		idx = 0;
		while ((idx < pbuff->bytes_read) && (!pline->eolat) && ++(pline->len))
			if ('\n' == pline->lnseg->charr[idx++])
				pline->eolat = idx;
	}
	return (1);
}
