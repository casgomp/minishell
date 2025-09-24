/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 11:33:10 by rhaas             #+#    #+#             */
/*   Updated: 2025/09/24 09:38:38 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include "ms_rmquotes_expand.h"

# include <stdbool.h>

typedef struct s_minishell_core_data
{
	int		exit_status;
	bool	interactive;
	int		fdscript;
	bool	exit;
	t_qes	default_qes;
	t_qed	default_qed;
}	t_cd;

int		get_cmdline_add_history(t_cd *pcd, char **pcmdline);
int		ms_builtins(t_cd *pcd, char **const args);
char	*ms_dict(char const *const key, void *dict_data);
int		ms_free_nullterm_array(char **arr);
int		process_cmdline(t_cd *pcd, char const *const in);
int		setup_stdin(t_cd *pcd, int const argc, char **const argv);
int		ms_free_nullterm_array(char **arr);

#endif // MINISHELL_H