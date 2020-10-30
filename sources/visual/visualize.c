/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visualize.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htrent <htrent@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/30 15:32:02 by htrent            #+#    #+#             */
/*   Updated: 2020/10/30 15:32:03 by htrent           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar_vs.h"

void	draw(t_arena *arena)
{
	erase();
	draw_borders();
	draw_info(arena);
	draw_core(arena);
	refresh();
}

void	play_cycle(t_arena *arena)
{
	if (arena->carriages_nbr)
	{
		arena->nbr_cycles++;
		arena->after_check++;
		ft_lstiter(arena->carriages, ft_run_carriages);
		ft_check_arena(arena);
	}
}

void	visualize(t_arena *arena)
{
	t_visual *visual;

	arena->verbose = 0;
	init_window();
	visual = init_visual(arena);
	init_map(arena);
	draw_carriages(arena);
	while ((arena->visual->button = getch()) != ESC)
	{
		handle_keyboard(arena);
		if (arena->visual->is_running && !arena->visual->debug
			&& (clock() >= calc_time_delay(arena->visual)))
		{
			arena->visual->time = clock();
			play_cycle(arena);
		}
		else if (arena->visual->button == ' ' && arena->visual->debug)
			play_cycle(arena);
		draw(arena);
	}
	endwin();
	free(visual);
}
