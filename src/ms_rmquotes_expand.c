/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_rmquotes_expand.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:12:30 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/23 17:01:38 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ms_rmquotes_expand.h"
#include "libft.h"

#include <stdbool.h>
#include <stdlib.h>

// minishell_isAllowedBashVariableCharacter
// https://thelinuxcode.com/bash-variable-name-rules-legal-illegal/
static bool	ms_isbvc(char const c, bool isfirstc)
{
	static char					firstc;
	static char const *const	special = "?";

	if (isfirstc)
	{
		firstc = c;
		if (ft_isdigit(c))
			return (false);
		if (ft_strchr(special, c))
			return (true);
	}
	else if (ft_strchr(special, firstc))
		return (false);
	return (ft_isalnum(c) || c == '_');
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

char	*ms_dict(char const *const key, void *data)
{
	t_cd *const	pcd = (t_cd *)data;

	if (!ft_strcmp(key, "?"))
		return (ft_itoa(pcd->exit_status));
	else
		return (getenv(key));
}

// returns...
//  1 if quote state was toggled
//  0 else
static int	ms_toggle_quote(char *const pc, t_qed const *const ptqed,
	t_uedata *const puedata)
{
	if ((!puedata->actvq && ft_strchr(ptqed->q, *pc))
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

static int	ms_escape(char *const pc, t_uedata *const puedata);
static int	ms_expand(char *const pc, int *const pidx,
				t_qed const *const ptqed, t_uedata *const puedata);
static int	ms_escape_expand(char *const pc, int *const pidx,
				t_qed const *const ptqed, t_uedata *const puedata);
static char	*ms_compose_uestr(t_uedata *const puedata);
static void	qed_cleanup(t_uedata *const puedata);

// Removes one level of quotes from the quoted string 'qutdstr'
// Different quote chars can be specified in the C-string 'q'
// (Only) if the quote uses the first char of 'q', escapes will be active
// and shell parameter expansion will happen.
// Most common values are: q="\"'" and esc='\\'
char	*ms_rmqts_esc_xpnd(char const *const qutdstr, t_qed tqed)
{
	int			idx;
	int			ret;
	char		*pc;
	t_uedata	uedata;

	// @rapha: This fun could/should be moved to libft!
	//         There is too many functions in this file anyways
	uedata = (t_uedata){ft_substr(qutdstr, 0, ft_strlen(qutdstr)), '\0',
		(t_strstart_len *) NULL, (t_strstart_len *) NULL, (char *) NULL, 0};
	if (!(uedata.instr && tqed.q && set_tsstl(&uedata, uedata.instr, 0)))
		return (qed_cleanup(&uedata), (char *) NULL);
	idx = 0;
	while (uedata.instr[idx])
	{
		pc = &(uedata.instr[idx]);
		if (ms_toggle_quote(pc, &tqed, &uedata))
			continue ;
		ret = ms_escape_expand(pc, &idx, &tqed, &uedata);
		if (ret == 1)
			continue ;
		else if (ret == -1)
			return (qed_cleanup(&uedata), (char *) NULL);
		++idx;
		++(uedata.tsl->len);
	}
	uedata.uestrlen += uedata.tsl->len;
	return (ms_compose_uestr(&uedata));
}

// returns...
//  1 if escape did happen
//  0 else
static int	ms_escape(char *const pc, t_uedata *const puedata)
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
static int	ms_expand(char *const pc, int *const pidx,
	t_qed const *const ptqed, t_uedata *const puedata)
{
	int		paramlen;
	char	*envvar;
	char	*envval;

	paramlen = 1; // including '$' !
	while (pc[paramlen] && ms_isbvc(pc[paramlen], 1 == paramlen))
		++paramlen;
	if (paramlen > 1)
	{
		envvar = ft_substr(pc, 1, paramlen - 1);
		if (envvar)
		{
			envval = ptqed->dict(envvar, ptqed->data);
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
static int	ms_escape_expand(char *const pc, int *const pidx,
				t_qed const *const ptqed, t_uedata *const puedata)
{
	if (puedata->actvq == ptqed->q[0] || !puedata->actvq)
	{
		if (ptqed->esc == *pc)
		{
			ms_escape(pc, puedata);
			return (0);
		}
		else if (ptqed->expnd == *pc)
		{
			return (ms_expand(pc, pidx, ptqed, puedata));
		}
	}
	return (0);
}

// Cleans up the linked list puedata->tsl_orig
// Because puedata->tsl_orig is initialised to zero, it's safe to run
//  this function even if nothing ever has been malloc'd
// Also puedata->tsl_orig is implicitly zero at the end of the fun in any case
static void	qed_cleanup(t_uedata *const puedata)
{
	puedata->tsl = puedata->tsl_orig;
	while (puedata->tsl_orig)
	{
		puedata->tsl_orig = puedata->tsl->next;
		free(puedata->tsl);
		puedata->tsl = puedata->tsl_orig;
	}
}

static char	*ms_compose_uestr(t_uedata *const puedata)
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
