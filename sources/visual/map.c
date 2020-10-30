/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htrent <htrent@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/30 15:31:49 by htrent            #+#    #+#             */
/*   Updated: 2020/10/30 15:31:50 by htrent           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar_vs.h"

void	update_map(t_arena *arena, t_carriage *carriage, int addr, int size)
{
	int s;

	while (size)
	{
		s = (addr + size - 1) % MEM_SIZE;
		if (s < 0)
			s += MEM_SIZE;
		arena->visual->map[s].value = carriage->owner->nbr_color;
		arena->visual->map[s].cycles_store = 50;
		size--;
	}
}
