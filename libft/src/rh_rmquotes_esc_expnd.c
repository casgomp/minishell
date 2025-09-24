/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rh_rmquotes_esc_expnd.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 16:38:01 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/24 17:06:28 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "rh_split_qed.h"
#include "rh_split_qed_intern.h"

#include <stdbool.h>
#include <stdlib.h>

static char	*compose_uestr(t_uedata *const puedata);
static int	escape(char *const pc, t_uedata *const puedata);
static int	expand(char *const pc, int *const pidx,
				t_qed const *const pqed, t_uedata *const puedata);
static int	escape_expand(char *const pc, int *const pidx,
				t_qed const *const pqed, t_uedata *const puedata);
static int	toggle_quote(char *const pc, t_qed const *const pqed,
				t_uedata *const puedata);
static int	set_tsstl(t_uedata *pue, char *const pcstart, size_t len);
static void	cleanup_uedata(t_uedata *const puedata);

// Removes one level of quotes from the quoted string 'qutdstr'
// Different quote chars can be specified in the C-string 'q'
// (Only) if the quote uses the first char of 'q', escapes will be active
// and shell parameter expansion will happen.
// Most common values are: q="\"'" and esc='\\'
char	*rh_rmqed(char const *const qutdstr, t_qed tqed)
{
	int			idx;
	int			ret;
	char		*pc;
	t_uedata	uedata;

	uedata = (t_uedata){ft_substr(qutdstr, 0, ft_strlen(qutdstr)), '\0',
		(t_strstart_len *) NULL, (t_strstart_len *) NULL, (char *) NULL, 0};
	if (!(uedata.instr && tqed.q && set_tsstl(&uedata, uedata.instr, 0)))
		return (cleanup_uedata(&uedata), (char *) NULL);
	idx = 0;
	while (uedata.instr[idx])
	{
		pc = &(uedata.instr[idx]);
		if (toggle_quote(pc, &tqed, &uedata))
			continue ;
		ret = escape_expand(pc, &idx, &tqed, &uedata);
		if (ret == 1)
			continue ;
		else if (ret == -1)
			return (cleanup_uedata(&uedata), (char *) NULL);
		++idx;
		++(uedata.tsl->len);
	}
	uedata.uestrlen += uedata.tsl->len;
	return (compose_uestr(&uedata));
}

char	**rh_split_rmqed(char const *const to_split, t_sqed const sqed)
{
	char **const	split = rh_split_quotes(to_split,
		(t_qes){.q = sqed.q, .esc = sqed.esc,
			.splt = sqed.splt, .specialsplt = sqed.specialsplt});
	char			*substr;
	int				idx;

	if (split)
	{
		idx = 0;
		while (split[idx])
		{
			substr = rh_rmqed(split[idx],
				(t_qed){.q = sqed.q, .esc = sqed.esc, .expnd = sqed.expnd,
					.dict = sqed.dict, .dict_data = sqed.dict_data,
					.validchar = sqed.validchar,
					.validchar_data = sqed.validchar_data});
			free(split[idx]);
			split[idx] = substr;
			++idx;
		}
	}
	return (split);
}

// returns...
//  1 if escape did happen
//  0 else
static int	escape(char *const pc, t_uedata *const puedata)
{
	if ((!puedata->actvq) || (puedata->actvq == *(pc + 1)))
	{
		ft_strlcpy(pc, pc + 1, ft_strlen(pc));
		return (1);
	}
	return (0);
}

// returns...
//  1 if expansion did happen
//  0 if expansion did not happen
// -1 if set_tsstl failed after expansion happened
static int	expand(char *const pc, int *const pidx,
	t_qed const *const pqed, t_uedata *const puedata)
{
	int		paramlen;
	char	*envvar;
	char	*envval;
	bool	isfirstchar;

	paramlen = 1; // including '$' !
	isfirstchar = true;
	while (pc[paramlen] && (*pqed->validchar)(pc[paramlen], &isfirstchar))
		++paramlen;
	if (paramlen > 1)
	{
		envvar = ft_substr(pc, 1, paramlen - 1);
		if (envvar)
		{
			envval = (*pqed->dict)(envvar, pqed->dict_data);
			if (envval)
				set_tsstl(puedata, envval, ft_strlen(envval));
			free(envvar);
		}
		*pidx += paramlen;
		if (!set_tsstl(puedata, &(puedata->instr[*pidx]), 0))
			return (-1);
		return (1);
	}
	return (0);
}

//  1 if expansion did happen
//  0 if expansion did not happen or escape happened
// -1 if set_tsstl failed after expansion happened
static int	escape_expand(char *const pc, int *const pidx,
				t_qed const *const pqed, t_uedata *const puedata)
{
	if (puedata->actvq == pqed->q[0] || !puedata->actvq)
	{
		if (pqed->esc == *pc)
		{
			escape(pc, puedata);
			return (0);
		}
		else if (pqed->expnd == *pc)
		{
			return (expand(pc, pidx, pqed, puedata));
		}
	}
	return (0);
}

// Cleans up the linked list puedata->tsl_orig
// Because puedata->tsl_orig is initialised to zero, it's safe to run
//  this function even if nothing ever has been malloc'd
// Also puedata->tsl_orig is implicitly zero at the end of the fun in any case
static void	cleanup_uedata(t_uedata *const puedata)
{
	if (puedata->instr)
		free(puedata->instr);
	puedata->tsl = puedata->tsl_orig;
	while (puedata->tsl_orig)
	{
		puedata->tsl_orig = puedata->tsl->next;
		free(puedata->tsl);
		puedata->tsl = puedata->tsl_orig;
	}
}

static char	*compose_uestr(t_uedata *const puedata)
{
	int	idx;

	puedata->uestr = malloc((puedata->uestrlen + 1) * sizeof(char));
	if (!puedata->uestr)
		return ((char *) NULL);
	puedata->tsl = puedata->tsl_orig;
	idx = 0;
	while (puedata->tsl)
	{
		ft_strlcpy(&(puedata->uestr[idx]), puedata->tsl->pcstart,
			puedata->tsl->len + 1);
		idx += puedata->tsl->len;
		puedata->tsl = puedata->tsl->next;
		free(puedata->tsl_orig);
		puedata->tsl_orig = puedata->tsl;
	}
	puedata->tsl_orig = (t_strstart_len *) NULL;
	free(puedata->instr);
	puedata->instr = (char *) NULL;
	return (puedata->uestr);
}

static int	set_tsstl(t_uedata *pue, char *const pcstart, size_t len)
{
	t_strstart_len	*new_tsstl;

	if (!(pue->tsl_orig) || (pue->tsl)->len)
	{
		new_tsstl = malloc(sizeof(t_strstart_len));
		if (!(pue->tsl_orig))
		{
			pue->tsl_orig = new_tsstl;
			pue->tsl = pue->tsl_orig;
		}
		else
		{
			pue->uestrlen += pue->tsl->len;
			pue->tsl->next = new_tsstl;
			pue->tsl = pue->tsl->next;
		}
	}
	*(pue->tsl) = (t_strstart_len){pcstart, len, (t_strstart_len *) NULL};
	return (pue->tsl != (t_strstart_len *) NULL);
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
