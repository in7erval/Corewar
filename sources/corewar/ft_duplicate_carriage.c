/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_duplicate_carriage.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htrent <htrent@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/30 15:47:55 by htrent            #+#    #+#             */
/*   Updated: 2020/10/30 15:47:57 by htrent           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

t_carriage	*duplicate_carriage(t_carriage *carriage)
{
	t_carriage *new_carriage;

	if (!(new_carriage = (t_carriage*)ft_memalloc(sizeof(t_carriage))))
		ft_exit("ERROR", NULL);
	ft_memmove(new_carriage, carriage, sizeof(t_carriage));
	new_carriage->pc = carriage->values[0];
	if (g_op_tab[carriage->op].op_code == 12)
		new_carriage->pc %= IDX_MOD;
	new_carriage->pc = ft_calculate_pc(carriage, new_carriage->pc);
	new_carriage->id = ++carriage->arena->carriage_index;
	new_carriage->regs[0] = -(carriage->owner->nbr);
	new_carriage->wait_cmd = 1;
	carriage->arena->carriages_nbr++;
	return (new_carriage);
}
