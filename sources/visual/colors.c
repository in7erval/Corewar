/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htrent <htrent@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/30 15:31:03 by htrent            #+#    #+#             */
/*   Updated: 2020/10/30 15:31:08 by htrent           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar_vs.h"

int	get_attribute(t_arena *arena, t_attr *attr, size_t cycles)
{
	if (cycles != (size_t)arena->nbr_cycles &&
		arena->cycles_to_die > 0
		&& attr->cycles_live > 0)
		attr->cycles_live--;
	if (cycles != (size_t)arena->nbr_cycles &&
		arena->cycles_to_die > 0 &&
		attr->cycles_store > 0)
		attr->cycles_store--;
	if (attr->cycles_live && attr->player)
		return (g_colors[attr->player->nbr_color + 10] | A_BOLD);
	else if (attr->cycles_store)
		return (g_colors[attr->value] | A_BOLD);
	else
		return (g_colors[attr->value]);
}
