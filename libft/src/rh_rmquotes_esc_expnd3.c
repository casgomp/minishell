/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rh_rmquotes_esc_expnd3.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 16:38:01 by rhaas             #+#    #+#             */
/*   Updated: 2025/10/22 18:40:34 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rh_split_qed.h"

t_qed	rh_sqed2qed(t_sqed const *const psqed)
{
	return ((t_qed){
		.q = psqed->q, .rmq = psqed->rmq,
		.esc = psqed->esc,
		.expstat = false, .expnd = psqed->expnd,
		.dict = psqed->dict, .dict_data = psqed->dict_data,
		.validchar = psqed->validchar, .validchar_data = psqed->validchar_data
	});
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
