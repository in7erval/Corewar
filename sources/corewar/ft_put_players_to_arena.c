/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_put_players_to_arena.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htrent <htrent@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/30 15:39:48 by htrent            #+#    #+#             */
/*   Updated: 2020/10/30 15:39:50 by htrent           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

t_carriage	*ft_create_carriage(t_arena *arena, t_player *player)
{
	t_carriage	*carriage;

	if (!(carriage = (t_carriage*)ft_memalloc(sizeof(*carriage))))
		ft_exit("ERROR", NULL);
	carriage->core = arena->core;
	carriage->wait_cmd = 1;
	carriage->arena = arena;
	carriage->owner = player;
	return (carriage);
}

void		ft_put_players_to_arena(t_arena *arena)
{
	t_carriage	*carriage;
	t_list		*players;
	t_list		*new_carriage;
	int			pc;

	pc = 0;
	players = arena->players;
	while (players)
	{
		carriage = ft_create_carriage(arena, (t_player *)players->content);
		carriage->pc = pc;
		carriage->id = ((t_player*)players->content)->nbr;
		carriage->regs[1] =
				ft_reverse_bytes(-((t_player*)players->content)->nbr);
		if (!(new_carriage = (ft_lstnew(carriage, sizeof(*carriage)))))
			ft_exit("ERROR", NULL);
		ft_lstadd(&arena->carriages, new_carriage);
		ft_memmove(arena->core + pc, ((t_player*)players->content)->code,
					ft_reverse_bytes(
					((t_player*)players->content)->header.prog_size));
		pc += MEM_SIZE / arena->carriages_nbr;
		players = players->next;
	}
	arena->live_id = -ft_reverse_bytes(carriage->regs[1]);
	carriage->arena->carriage_index = carriage->arena->carriages_nbr;
}
