/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_divdefs.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 12:19:29 by rhaas             #+#    #+#             */
/*   Updated: 2025/11/17 20:39:51 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSH_DIVDEFS_H
# define MSH_DIVDEFS_H

# define ENV_XTRA_ELEMS 8

typedef enum e_redirect
{
	fd2file_trunc,
	fd2file_append,
	file2fd,
	heredoc2fd,
	rd_default,
}	t_rd;

typedef enum e_prompt_type
{
	pt_std = 0,
	pt_contd,
}	t_pt;

typedef enum e_cmd_status
{
	cs_uninitialised = 0,
	cs_cmds_ok,
	cs_unfinished_pipe,
	cs_syntax_err,
}	t_cs;

#endif // MSH_DIVDEFS_H