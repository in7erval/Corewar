/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_arena.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/02 01:46:13 by majosue           #+#    #+#             */
/*   Updated: 2020/11/05 05:44:12 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

void	ft_mark_death_carriages(t_list *carriages)
{
	t_carriage *carriage;

	carriage = carriages->content;
	if (carriage->death == 0 && (carriage->arena->nbr_cycles
		- carriage->last_live_cycle) >= carriage->arena->cycles_to_die)
	{
		if (carriage->arena->v)
			remove_carriage(carriage->arena, carriage);
		carriage->death = 1;
		carriage->arena->carriages_nbr--;
		if (carriage->arena->verbose & 8)
			ft_printf("Process %d hasn't lived for %d cycles (CTD %d)\n",
				carriage->id, carriage->arena->nbr_cycles -
				carriage->last_live_cycle, carriage->arena->cycles_to_die);
	}
}

void	ft_reset_lives(t_list *players_list)
{
	t_player *player;

	while (players_list)
	{
		player = (t_player *)players_list->content;
		player->current_lives = 0;
		players_list = players_list->next;
	}
}

int		ft_check_arena(t_arena *arena)
{
	if (arena->cycles_to_die > 0 &&
	((arena->nbr_cycles - arena->cycle_change_cycles_to_die) &&
	(arena->nbr_cycles - arena->cycle_change_cycles_to_die) %
	arena->cycles_to_die))
		return (0);
	ft_lstiter(arena->carriages, ft_mark_death_carriages);
	if (arena->carriages_nbr == 0)
		return (1);
	if (arena->live_nbr >= NBR_LIVE || arena->checks_nbr >= MAX_CHECKS)
	{
		arena->cycles_to_die -= CYCLE_DELTA;
		arena->checks_nbr = 0;
		arena->cycle_change_cycles_to_die = arena->nbr_cycles;
		if (arena->verbose & 2)
			ft_printf("Cycle to die is now %d\n", arena->cycles_to_die);
	}
	arena->checks_nbr++;
	arena->after_check = 0;
	arena->live_nbr = 0;
	ft_reset_lives(arena->players);
	return (0);
}
