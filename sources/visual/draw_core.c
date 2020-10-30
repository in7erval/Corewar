/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_core.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htrent <htrent@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/30 15:31:21 by htrent            #+#    #+#             */
/*   Updated: 2020/10/30 15:31:22 by htrent           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar_vs.h"

void	remove_carriage(t_arena *arena, t_carriage *carriage)
{
	if (arena->visual->map[carriage->pc].value >= 5 &&
		arena->visual->map[carriage->pc].value <= 9)
		arena->visual->map[carriage->pc].value -= 5;
}

void	place_carriage(t_arena *arena, t_carriage *carriage)
{
	if (arena->visual->map[carriage->pc].value >= 0 &&
		arena->visual->map[carriage->pc].value <= MAX_PLAYERS)
		arena->visual->map[carriage->pc].value += 5;
}

void	draw_carriages(t_arena *arena)
{
	t_carriage	*carriage;
	t_list		*carriages;

	carriages = arena->carriages;
	while (carriages)
	{
		carriage = (t_carriage *)carriages->content;
		place_carriage(arena, carriage);
		carriages = carriages->next;
	}
}

void	draw_core(t_arena *arena)
{
	static size_t	cycles = 0;
	size_t			i;
	size_t			x;
	size_t			y;
	int				attr;

	i = 0;
	x = 2;
	y = 2;
	while (i < MEM_SIZE)
	{
		if (i / DUMP_WIDTH && !(i % DUMP_WIDTH))
		{
			y++;
			x = 2;
		}
		x++;
		attr = get_attribute(arena, &(arena->visual->map[i]), cycles);
		attron(attr);
		mvprintw(y, x, "%.2x", ((unsigned char *)(arena->core))[i]);
		attroff(attr);
		x += 2;
		i++;
	}
	cycles = arena->nbr_cycles;
}
