/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rh_split_qed_intern.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 17:14:25 by rhaas             #+#    #+#             */
/*   Updated: 2025/10/22 18:53:22 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RH_SPLIT_QED_INTERN_H
# define RH_SPLIT_QED_INTERN_H

# include <stddef.h>

// Split with quotes and escapes
// -----------------------------
typedef struct s_startlen
{
	int					start;
	size_t				len;
	struct s_startlen	*next;
}	t_startlen;

typedef struct s_splitparams
{
	char		**split;
	t_startlen	*tsl_orig;
	t_startlen	*tsl;
	size_t		n_slelem;
	char		activeq;
}	t_splprms;

int		set_tsl(t_splprms *const ptsp, int idx);
char	**transform_list(t_splprms *const ptsp, char const *const s);
void	cleanup_splt(t_splprms *const ptsp);

// remove quotes, escapes and expand params
// ----------------------------------------
typedef struct s_strstart_len
{
	bool					newstr;
	bool					expanded;
	char					*pcstart;
	size_t					len;
	struct s_strstart_len	*next;
}	t_strstart_len;

typedef struct s_unquted_expanded_split_data
{
	char			*instr;
	char			actvq;
	bool			expansion;
	t_strstart_len	*tsl_orig;
	t_strstart_len	*tsl;
	char			*uestr;
	size_t			uestrlen;
}	t_uedata;

typedef struct s_expand_wordsplit
{
	int		paramlen;
	char	*envvar;
	char	*envval;
	bool	isfirstchar;
}	t_ews;

int		set_tsstl(t_uedata *pue, char *const pcstart, size_t len,
			bool expanded);
int		escape(char *const pc, int *const pidx, t_qed const *const pqed,
			t_uedata *const puedata);
int		expand(char *const pc, int *const pidx,
			t_qed *const pqed, t_uedata *const puedata);
int		escape_expand(char *const pc, int *const pidx,
			t_qed *const pqed, t_uedata *const puedata);

#endif // RH_SPLIT_QED_INTERN_H
