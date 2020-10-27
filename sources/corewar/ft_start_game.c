/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_start_game.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/02 00:13:53 by majosue           #+#    #+#             */
/*   Updated: 2020/10/02 06:09:18 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

void	ft_check_op(t_carriage *carriage)
{
	if (carriage->op > 0 && carriage->op < 17)
	{
		carriage->cycles_to_exec = g_op_tab[carriage->op].cycles_to_exec - 2;
		carriage->wait_cmd = 0;
	}
	else
	{
		if (carriage->arena->v)
			remove_carriage(carriage->arena, carriage);
		carriage->pc = (carriage->pc + 1) % MEM_SIZE;
		if (carriage->arena->v)
			place_carriage(carriage->arena, carriage);
	}
}

void	ft_run_op(t_carriage *carriage)
{
	t_arg_byte	arg;
	int			args[3];
	int			result;

	arg.byte = carriage->core[(carriage->pc + 1) % MEM_SIZE];
	args[0] = arg.value.at8;
	args[1] = arg.value.at6;
	args[2] = arg.value.at4;
	result = g_op_tab[carriage->op].f(carriage, args);
	if (carriage->arena->verbose & 4)
		ft_print_op(carriage, result, args);
}

void	ft_run_carriages(t_list *carriages)
{
	t_carriage *carriage;

	carriage = carriages->content;
	if (carriage->death)
		return ;
	if (!carriage->cycles_to_exec)
	{
		if (carriage->wait_cmd)
		{
			carriage->op = carriage->core[carriage->pc];
			ft_check_op(carriage);
		}
		else
			ft_run_op(carriage);
		return ;
	}
	carriage->cycles_to_exec--;
}

void	ft_introduce(t_list *players)
{
	t_player *player;

	player = players->content;
	ft_printf("* Player %d, weighing %d bytes, \"%s\" (\"%s\") !\n",
			player->nbr, ft_reverse_bytes(player->header.prog_size),
			player->header.prog_name, player->header.comment);
}

void	ft_start_game(t_arena *arena)
{
	ft_printf("Introducing contestants...\n");
	ft_lstiter(arena->players, ft_introduce);
	while (1)
	{
		if (arena->dump_nbr_cycles &&
		arena->dump_nbr_cycles == arena->nbr_cycles)
		{
			ft_print_memory(arena->core, MEM_SIZE);
			break ;
		}
		(arena->nbr_cycles)++;
		if (arena->verbose & 2)
			ft_printf("It is now cycle %d\n", arena->nbr_cycles);
		ft_lstiter(arena->carriages, ft_run_carriages);
		if (ft_check_arena(arena))
		{
			if (ft_get_player(arena, arena->live_id))
				ft_printf("Contestant %d, \"%s\", has won !\n", arena->live_id,
				ft_get_player(arena, arena->live_id)->header.prog_name);
			break ;
		}
	}
}
