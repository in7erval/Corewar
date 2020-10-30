/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_1_8.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/02 02:36:41 by majosue           #+#    #+#             */
/*   Updated: 2020/10/02 02:58:47 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

int	ft_live(t_carriage *carriage, int args[3])
{
	t_player	*player;
	int			id;

	args[0] = DIR_CODE;
	ft_load_params(carriage, args, 1, 0);
	ft_load_values(carriage, args);
	id = -carriage->values[0];
	if ((player = ft_get_player(carriage->arena, id)))
	{
		player->current_lives++;
		player->last_live = carriage->arena->nbr_cycles;
		if (carriage->arena->verbose & 1)
			ft_printf("A process shows that player %d (%s) is alive\n",
		id, player->header.prog_name);
		carriage->arena->live_id = id;
		if (carriage->arena->v)
		{
			carriage->arena->visual->map[carriage->pc].cycles_live = 50;
			carriage->arena->visual->map[carriage->pc].player = player;
		}
	}
	ft_skip_args(carriage, args, 1);
	carriage->arena->live_nbr++;
	carriage->last_live_cycle = carriage->arena->nbr_cycles;
	return (EXIT_SUCCESS);
}

int	ft_ld_lld(t_carriage *carriage, int args[3])
{
	int mod;
	int result;

	if (!((args[0] == DIR_CODE || args[0] == IND_CODE) &&
	args[1] == REG_CODE && ft_is_valid_regs(carriage, args)))
	{
		ft_skip_args(carriage, args, 2);
		return (EXIT_FAILURE);
	}
	mod = g_op_tab[carriage->op].op_code == 2 ? 1 : 0;
	ft_load_params(carriage, args, mod, 1);
	ft_load_values(carriage, args);
	result = ft_reverse_bytes(carriage->values[0]);
	ft_memmove_circle(carriage->params[1],
	&result, carriage, REG_SIZE);
	ft_load_values(carriage, args);
	carriage->carry = carriage->values[1] ? 0 : 1;
	ft_skip_args(carriage, args, 2);
	return (EXIT_SUCCESS);
}

int	ft_st(t_carriage *carriage, int args[3])
{
	if (!(args[0] == REG_CODE && (args[1] == REG_CODE ||
	args[1] == IND_CODE) && ft_is_valid_regs(carriage, args)))
	{
		ft_skip_args(carriage, args, 2);
		return (EXIT_FAILURE);
	}
	ft_load_params(carriage, args, 1, 1);
	ft_memmove_circle(carriage->params[1], carriage->params[0],
			carriage, REG_SIZE);
	ft_load_values(carriage, args);
	if ((unsigned char *)carriage->params[1] - carriage->core >= 0 &&
		(unsigned char *)carriage->params[1] - carriage->core < MEM_SIZE)
		carriage->values[1] = carriage->ind_val[1];
	if (carriage->arena->v)
		update_map(carriage->arena, carriage,
	((unsigned char *)carriage->params[1] - carriage->core), DIR_SIZE);
	ft_skip_args(carriage, args, 2);
	return (EXIT_SUCCESS);
}

int	ft_add_sub(t_carriage *carriage, int args[3])
{
	int result;

	if (!(args[0] == REG_CODE && args[1] == REG_CODE &&
		args[2] == REG_CODE && ft_is_valid_regs(carriage, args)))
	{
		ft_skip_args(carriage, args, 2);
		return (EXIT_FAILURE);
	}
	ft_load_values(carriage, args);
	if (g_op_tab[carriage->op].op_code == 4)
		carriage->values[2] = carriage->values[0] + carriage->values[1];
	else
		carriage->values[2] = carriage->values[0] - carriage->values[1];
	result = ft_reverse_bytes(carriage->values[2]);
	ft_memmove(carriage->params[2], &result, REG_SIZE);
	ft_skip_args(carriage, args, 2);
	carriage->carry = carriage->values[2] ? 0 : 1;
	return (EXIT_SUCCESS);
}

int	ft_and_or_xor(t_carriage *carriage, int args[3])
{
	int result;

	if (!(args[0] != 0 && args[1] != 0 && args[2] == REG_CODE &&
		ft_is_valid_regs(carriage, args)))
	{
		ft_skip_args(carriage, args, 2);
		return (EXIT_FAILURE);
	}
	ft_load_params(carriage, args, 1, 1);
	ft_load_values(carriage, args);
	if (g_op_tab[carriage->op].op_code == 6)
		carriage->values[2] = carriage->values[0] & carriage->values[1];
	else if (g_op_tab[carriage->op].op_code == 7)
		carriage->values[2] = carriage->values[0] | carriage->values[1];
	else
		carriage->values[2] = carriage->values[0] ^ carriage->values[1];
	result = ft_reverse_bytes(carriage->values[2]);
	ft_memmove_circle(carriage->params[2], &result, carriage, REG_SIZE);
	ft_skip_args(carriage, args, 2);
	carriage->carry = carriage->values[2] ? 0 : 1;
	return (EXIT_SUCCESS);
}
