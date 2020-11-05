/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_and_usage.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/30 15:41:13 by htrent            #+#    #+#             */
/*   Updated: 2020/11/05 05:46:20 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

void		ft_init_arena(t_arena *arena)
{
	arena->carriages_nbr = 0;
	arena->carriage_index = 0;
	arena->carriages = NULL;
	arena->dump_nbr_cycles = -1;
	arena->players = NULL;
	arena->cycles_to_die = CYCLE_TO_DIE;
	arena->cycle_change_cycles_to_die = 0;
	arena->live_nbr = 0;
	arena->nbr_cycles = 0;
	arena->checks_nbr = 0;
	arena->verbose = 0;
	arena->visual = NULL;
	arena->v = 0;
	arena->a = 0;
	arena->after_check = 0;
	arena->legacy = 1;
	ft_bzero(arena->core, MEM_SIZE);
}

int			print_usage(void)
{
	ft_printf("\
Usage: ./corewar [-a -v -dump N -verbose N -legacy N] [[-n N] champion1.cor]\
...\n\
	-a : Prints output from \"aff\" (Default is to hide it)\n\
	-v : Visual mode (with ncurses)\n\
	-dump N	: Dumps memory after N cycles then exits\n\
	-verbose N : Verbosity levels, can be added together to enable several\n\
		- 0 : Show only essentials (default)\n\
		- 1 : Show lives\n\
		- 2 : Show cycles\n\
		- 4 : Show operations (Params are NOT litteral ...)\n\
		- 8 : Show deaths\n\
	-legacy N : Compability with zaz corewar\n\
		- 1 : Repeat subject corewar lld bug (default)\n\
		- 0 : lld bug free\n\
	-n N : Set unic champion number\n");
	return (1);
}
