/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_tools.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/02 04:23:59 by majosue           #+#    #+#             */
/*   Updated: 2020/10/02 04:32:46 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

/*
**	Move pc to next pos
*/

void	ft_skip_args(t_carriage *carriage, int args[3], int addon)
{
	int i;
	int skip;

	i = 0;
	skip = 0;
	while (i < g_op_tab[carriage->op].max_params)
	{
		skip += g_params[g_op_tab[carriage->op].short_dir][args[i]];
		i++;
	}
	if (carriage->arena->v)
		remove_carriage(carriage->arena, carriage);
	carriage->pc = (carriage->pc + skip + addon) % MEM_SIZE;
	if (carriage->arena->v)
		place_carriage(carriage->arena, carriage);
	carriage->wait_cmd = 1;
}

/*
**	Check valid registry number
**	store adress of registry to carriage->params
**	store number to carriage->reg_nbrs
*/

int		ft_is_valid_regs(t_carriage *carriage, int args[3])
{
	int i;
	int pc;

	i = 0;
	pc = (carriage->pc + 2) % MEM_SIZE;
	while (i < g_op_tab[carriage->op].max_params)
	{
		if (args[i] == REG_CODE)
		{
			if (carriage->core[pc] <= 0 || carriage->core[pc] > REG_NUMBER)
				return (0);
			carriage->params[i] = &(carriage->regs[carriage->core[pc]]);
			carriage->reg_nbrs[i] = carriage->core[pc];
		}
		pc = (pc + g_params[g_op_tab[carriage->op].short_dir][args[i]])
		% MEM_SIZE;
		i++;
	}
	return (1);
}

int		ft_calculate_pc(t_carriage *carriage, int addon)
{
	int pc;

	addon = addon % MEM_SIZE;
	pc = carriage->pc;
	pc += addon;
	if (pc < 0)
		pc = MEM_SIZE + pc;
	else
		pc = pc % MEM_SIZE;
	return (pc);
}

/*
**	Store adress of args to carriage->params
**	store IND values to carriage->ind_val
*/

void	ft_load_params(t_carriage *carriage,
					const int args[3], int mod, int arg_bytes)
{
	int		pc;
	int		ind_pc;
	int		i;
	short	ind;

	pc = (carriage->pc + 1 + arg_bytes) % MEM_SIZE;
	i = 0;
	while (i < g_op_tab[carriage->op].max_params)
	{
		if (args[i] == DIR_CODE)
			carriage->params[i] = &carriage->core[pc];
		if (args[i] == IND_CODE)
		{
			ft_memmove_circle(&ind, &carriage->core[pc],
			carriage, sizeof(short));
			ind = ft_reverse_bytes_short(ind);
			carriage->ind_val[i] = ind;
			ind = mod ? ind % IDX_MOD : ind;
			ind_pc = ft_calculate_pc(carriage, ind);
			carriage->params[i] = &carriage->core[ind_pc];
		}
		pc = (pc + g_params[g_op_tab[carriage->op].short_dir][args[i]])
		% MEM_SIZE;
		i++;
	}
}

/*
**	Store values at carriage->params in carriage->values
**	use DIR_SIZE instead others (we already get addresses in load_params)
**	use "g_op_tab[carriage->op].op_code == 0x0d ? 2 : size;"
**	for repeat behaviour of original corewar
**	op 0x0d lld read 2 bytes on IND address
*/

void	ft_load_values(t_carriage *carriage, int args[3])
{
	int		i;
	int		size;
	short	buffer;

	i = 0;
	while (i < g_op_tab[carriage->op].max_params)
	{
		size = args[i] == IND_CODE || args[i] == REG_CODE ? DIR_SIZE :
		g_params[g_op_tab[carriage->op].short_dir][args[i]];
		size = args[i] == IND_CODE && g_op_tab[carriage->op].op_code == 0x0d &&
		carriage->arena->legacy ? 2 : size;
		if (size < DIR_SIZE)
		{
			ft_memmove_circle(&buffer, carriage->params[i], carriage, size);
			buffer = ft_reverse_bytes_short(buffer);
			carriage->values[i] = buffer;
		}
		else
		{
			ft_memmove_circle(&carriage->values[i], carriage->params[i],
			carriage, size);
			carriage->values[i] = ft_reverse_bytes(carriage->values[i]);
		}
		i++;
	}
}
