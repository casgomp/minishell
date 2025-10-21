/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rh_rmquotes_esc_expnd.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 16:38:01 by rhaas             #+#    #+#             */
/*   Updated: 2025/10/21 10:01:03 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "rh_split_qed.h"
#include "rh_split_qed_intern.h"

#include <stdbool.h>
#include <stdlib.h>

static char	**compose_uestr(t_uedata *const puedata);
static int	toggle_quote(char *const pc, t_qed const *const pqed,
				t_uedata *const puedata);
static void	cleanup_uedata(t_uedata *const puedata);

// Removes one level of quotes from the quoted string 'qutdstr'
// Different quote chars can be specified in the C-string 'q'
// (Only) if the quote uses the first char of 'q', escapes will be active
// and shell parameter expansion will happen.
// Most common values are: q="\"'" and esc='\\'
char	**rh_rmqed(char const *const qutdstr, t_qed tqed)
{
	int			idx;
	int			ret;
	t_uedata	uedata;

	uedata = (t_uedata){.instr = ft_substr(qutdstr, 0, ft_strlen(qutdstr)),
		.actvq = '\0', .tsl_orig = (t_strstart_len *) NULL,
		.tsl = (t_strstart_len *) NULL, .uestrs = (char**) NULL, .uestrlen = 0};
	if (!(uedata.instr && tqed.q && set_tsstl(&uedata, uedata.instr, 0)))
		return (cleanup_uedata(&uedata), (char **) NULL);
	idx = 0;
	while (uedata.instr[idx])
	{
		if (toggle_quote(&uedata.instr[idx], &tqed, &uedata))
			continue ;
		ret = escape_expand(&uedata.instr[idx], &idx, &tqed, &uedata);
		if (ret > 0)
			continue ;
		else if (ret < 0)
			return (cleanup_uedata(&uedata), (char **) NULL);
		++idx;
		++(uedata.tsl->len);
	}
	uedata.uestrlen += uedata.tsl->len;
	return (compose_uestr(&uedata));
}

// Cleans up the linked list puedata->tsl_orig
// Because puedata->tsl_orig is initialised to zero, it's safe to run
//  this function even if nothing ever has been malloc'd
// Also puedata->tsl_orig is implicitly zero at the end of the fun in any case
static void	cleanup_uedata(t_uedata *const puedata)
{
	if (puedata->instr)
		free(puedata->instr);
	puedata->instr = (char *) NULL;
	while (puedata->tsl_orig)
	{
		puedata->tsl = puedata->tsl_orig->next;
		free(puedata->tsl_orig);
		puedata->tsl_orig = puedata->tsl;
	}
	puedata->tsl_orig = (t_strstart_len *) NULL;
}

static bool	init_compose_uestr(t_uedata *const puedata)
{
	size_t		numparams;

	numparams = 0;
	puedata->tsl = puedata->tsl_orig;
	while (puedata->tsl)
	{
		if (puedata->tsl->newstr)
			++numparams;
		puedata->tsl = puedata->tsl->next;
	}
	puedata->uestrs = malloc((numparams + 1) * sizeof(char *));
	if (!puedata->uestrs)
		return (false);
	puedata->uestrs[numparams] = (char *) NULL;
	return (true);
}

static char	**compose_uestr(t_uedata *const puedata)
{
	size_t		paramidx;
	size_t		idx;
	char *const	buffer = malloc((puedata->uestrlen + 1) * sizeof(char));

	if (!buffer)
		return ((char **) NULL);
	if (!init_compose_uestr(puedata))
		return (free(buffer), cleanup_uedata(puedata), (char **) NULL);
	puedata->tsl = puedata->tsl_orig;
	paramidx = -1;
	while (puedata->tsl)
	{
		if (puedata->tsl->newstr)
			idx = 0;
		ft_strlcpy(&(buffer[idx]), puedata->tsl->pcstart,
			puedata->tsl->len + 1);
		idx += puedata->tsl->len;
		puedata->tsl = puedata->tsl->next;
		if (!puedata->tsl || puedata->tsl->newstr)
			puedata->uestrs[++paramidx] = ft_strdup(buffer);
	}
	return (free(buffer), cleanup_uedata(puedata), puedata->uestrs);
}

// returns...
//  1 if quote state was toggled
//  0 else
static int	toggle_quote(char *const pc, t_qed const *const pqed,
	t_uedata *const puedata)
{
	if ((!puedata->actvq && ft_strchr(pqed->q, *pc))
		|| (puedata->actvq && *pc == puedata->actvq))
	{
		if (!puedata->actvq)
			puedata->actvq = *pc;
		else if (puedata->actvq)
			puedata->actvq = '\0';
		ft_strlcpy(pc, pc + 1, ft_strlen(pc));
		return (1);
	}
	return (0);
}
