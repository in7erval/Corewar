/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_op.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/02 02:02:06 by majosue           #+#    #+#             */
/*   Updated: 2020/10/02 02:08:00 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

void	ft_print_sti(t_carriage *carriage)
{
	ft_printf("r%d %d %d\n", carriage->reg_nbrs[0], carriage->values[1],
	carriage->values[2]);
	ft_printf("       | -> store to %d + %d = %d (with pc and mod %d)\n",
	carriage->values[1], carriage->values[2], carriage->binary_op_result,
	carriage->target_pc);
}

void	ft_print_ldi(t_carriage *carriage)
{
	ft_printf("%d %d r%d\n", carriage->values[0], carriage->values[1],
	carriage->reg_nbrs[2]);
	ft_printf("       | -> load from %d + %d = %d (with pc and mod %d)\n",
	carriage->values[0], carriage->values[1], carriage->binary_op_result,
	carriage->target_pc);
}

void	ft_print_zjump(t_carriage *carriage)
{
	if (carriage->carry == 0)
		ft_printf("%d FAILED\n", carriage->values[0]);
	else
		ft_printf("%d OK\n", carriage->values[0]);
}

void	ft_print_common_op(t_carriage *carriage, int args[3])
{
	int i;

	i = -1;
	while (++i < g_op_tab[carriage->op].max_params)
	{
		if (args[i] == REG_CODE)
			ft_printf("r%d ", carriage->reg_nbrs[i]);
		else
			ft_printf("%d ", carriage->values[i]);
	}
	ft_printf("\n");
}

void	ft_print_op(t_carriage *carriage, int result, int args[3])
{
	if (result && g_op_tab[carriage->op].op_code != 9)
		return ;
	ft_printf("P%5d | %s ", carriage->id, g_op_tab[carriage->op].name);
	if (g_op_tab[carriage->op].op_code == 12 ||
	g_op_tab[carriage->op].op_code == 15)
		ft_printf("%d (%d)\n", carriage->values[0], carriage->target_pc);
	else if (g_op_tab[carriage->op].op_code == 11)
		ft_print_sti(carriage);
	else if (g_op_tab[carriage->op].op_code == 10
	|| g_op_tab[carriage->op].op_code == 14)
		ft_print_ldi(carriage);
	else if (g_op_tab[carriage->op].op_code == 6 ||
	g_op_tab[carriage->op].op_code == 7 ||
	g_op_tab[carriage->op].op_code == 8)
		ft_printf("%d %d r%d\n", carriage->values[0], carriage->values[1],
		carriage->reg_nbrs[2]);
	else if (g_op_tab[carriage->op].op_code == 9)
		ft_print_zjump(carriage);
	else
		ft_print_common_op(carriage, args);
}
