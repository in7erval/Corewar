/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_9_16.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/02 03:01:14 by majosue           #+#    #+#             */
/*   Updated: 2020/10/02 04:17:01 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

int	ft_zjmp(t_carriage *carriage, int args[3])
{
	int pc;

	args[0] = DIR_CODE;
	ft_load_params(carriage, args, 1, 0);
	ft_load_values(carriage, args);
	pc = carriage->values[0];
	pc = pc % IDX_MOD;
	pc = ft_calculate_pc(carriage, pc);
	if (carriage->carry == 0)
	{
		ft_skip_args(carriage, args, 1);
		return (EXIT_FAILURE);
	}
	if (carriage->arena->v)
		remove_carriage(carriage->arena, carriage);
	carriage->pc = pc;
	if (carriage->arena->v)
		place_carriage(carriage->arena, carriage);
	carriage->wait_cmd = 1;
	return (EXIT_SUCCESS);
}

int	ft_ldi_lldi(t_carriage *carriage, int args[3])
{
	int pc;

	if (!(args[0] != 0 && (args[1] == DIR_CODE ||
args[1] == REG_CODE) && args[2] == REG_CODE &&
ft_is_valid_regs(carriage, args)))
	{
		ft_skip_args(carriage, args, 2);
		return (EXIT_FAILURE);
	}
	ft_load_params(carriage, args, 1, 1);
	ft_load_values(carriage, args);
	carriage->values[2] = carriage->values[0] + carriage->values[1];
	carriage->binary_op_result = carriage->values[2];
	carriage->values[2] = g_op_tab[carriage->op].op_code == 10 ?
	carriage->values[2] % IDX_MOD : carriage->values[2];
	carriage->target_pc = carriage->values[2] + carriage->pc;
	pc = ft_calculate_pc(carriage, carriage->values[2]);
	ft_memmove_circle(carriage->params[2], &carriage->core[pc],
	carriage, REG_SIZE);
	if (g_op_tab[carriage->op].op_code == 14)
		carriage->carry = *(int *)carriage->params[2] ? 0 : 1;
	ft_skip_args(carriage, args, 2);
	return (EXIT_SUCCESS);
}

int	ft_sti(t_carriage *carriage, int args[3])
{
	int pc;

	if (!(args[0] == REG_CODE && args[1] != 0 &&
		(args[2] == DIR_CODE || args[2] == REG_CODE) &&
		ft_is_valid_regs(carriage, args)))
	{
		ft_skip_args(carriage, args, 2);
		return (EXIT_FAILURE);
	}
	ft_load_params(carriage, args, 1, 1);
	ft_load_values(carriage, args);
	carriage->values[0] = carriage->values[1] + carriage->values[2];
	carriage->binary_op_result = carriage->values[0];
	carriage->values[0] = carriage->values[0] % IDX_MOD;
	carriage->target_pc = carriage->values[0] + carriage->pc;
	pc = ft_calculate_pc(carriage, carriage->values[0]);
	ft_memmove_circle(&carriage->core[pc],
	carriage->params[0], carriage, REG_SIZE);
	if (carriage->arena->v)
		update_map(carriage->arena, carriage, pc, DIR_SIZE);
	ft_skip_args(carriage, args, 2);
	return (EXIT_SUCCESS);
}

int	ft_fork_lfork(t_carriage *carriage, int args[3])
{
	int			mod;
	t_carriage	*new_carriage;
	t_list		*new_node;

	mod = g_op_tab[carriage->op].op_code == 12 ? 1 : 0;
	args[0] = DIR_CODE;
	ft_load_params(carriage, args, mod, 0);
	ft_load_values(carriage, args);
	new_carriage = duplicate_carriage(carriage);
	carriage->target_pc = new_carriage->pc + carriage->pc;
	if (!(new_node = ft_lstnew(new_carriage, sizeof(*new_carriage))))
		ft_exit("ERROR", NULL);
	ft_lstadd(&carriage->arena->carriages, new_node);
	ft_skip_args(carriage, args, 1);
	return (EXIT_SUCCESS);
}

int	ft_aff(t_carriage *carriage, int args[3])
{
	int c;

	if (!(args[0] == REG_CODE && ft_is_valid_regs(carriage, args)))
	{
		ft_skip_args(carriage, args, 2);
		return (EXIT_FAILURE);
	}
	ft_load_params(carriage, args, 1, 1);
	ft_load_values(carriage, args);
	c = carriage->values[0];
	c = c % 256;
	if (carriage->arena->v)
	{
		carriage->arena->visual->aff = c;
		carriage->arena->visual->has_aff = 1;
	}
	else if (carriage->arena->a)
		ft_printf("Aff: %c\n", c);
	ft_skip_args(carriage, args, 2);
	return (EXIT_SUCCESS);
}
