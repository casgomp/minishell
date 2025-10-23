/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rh_rmquotes_esc_expnd3.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 16:38:01 by rhaas             #+#    #+#             */
/*   Updated: 2025/10/23 13:40:14 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rh_split_qed.h"

#include "stdlib.h"

t_qed	rh_sqed2qed(t_sqed const *const psqed)
{
	return ((t_qed){
		.q = psqed->q,
		.esc = psqed->esc,
		.rmqe = psqed->rmqe,
		.expnd = psqed->expnd, .expstat = false, 
		.dict = psqed->dict, .dict_data = psqed->dict_data,
		.validchar = psqed->validchar, .validchar_data = psqed->validchar_data
	});
}

t_qed	rh_sqed2qed_rmonly(t_sqed const *const psqed)
{
	return ((t_qed){
		.q = psqed->q,
		.esc = psqed->esc,
		.rmqe = true,
		.expnd = '\0', .expstat = false,
		.dict = NULL, .dict_data = (void *) NULL,
		.validchar = NULL, .validchar_data = (void *) NULL});
}

t_qes	rh_sqed2qes(t_sqed const *const psqed)
{
	return ((t_qes){
		.q = psqed->q,
		.esc = psqed->esc,
		.blank = psqed->blank,
		.metachr = psqed->metachr
	});
}

// wordsplit special qes
t_qes	rh_sqed2qes_ws(t_sqed const *const psqed)
{
	return ((t_qes){
		.q = psqed->q,
		.esc = '\0',
		.blank = psqed->ifs,
		.metachr = ""});
}
