/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/08 20:32:21 by majosue           #+#    #+#             */
/*   Updated: 2020/10/02 06:04:03 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

t_op	g_op_tab[17] =
{
	{0, 0, {0}, 0, 0, 0, 0, 0, 0},
	{"live", 1, {T_DIR}, 1, 10, "alive", 0, 0, ft_live},
	{"ld", 2, {T_DIR | T_IND, T_REG}, 2, 5, "load", 1, 0, ft_ld_lld},
	{"st", 2, {T_REG, T_IND | T_REG}, 3, 5, "store", 1, 0, ft_st},
	{"add", 3, {T_REG, T_REG, T_REG}, 4, 10, "addition", 1, 0, ft_add_sub},
	{"sub", 3, {T_REG, T_REG, T_REG}, 5, 10, "soustraction", 1, 0, ft_add_sub},
	{"and", 3, {T_REG | T_DIR | T_IND, T_REG | T_IND | T_DIR, T_REG}, 6, 6,
		"et (and r1, r2, r3 r1&r2 -> r3", 1, 0, ft_and_or_xor},
	{"or", 3, {T_REG | T_IND | T_DIR, T_REG | T_IND | T_DIR, T_REG}, 7, 6,
		"ou  (or r1, r2, r3 r1|r2 -> r3", 1, 0, ft_and_or_xor},
	{"xor", 3, {T_REG | T_IND | T_DIR, T_REG | T_IND | T_DIR, T_REG}, 8, 6,
		"ou (xor r1, r2, r3 r1^r2 -> r3", 1, 0, ft_and_or_xor},
	{"zjmp", 1, {T_DIR}, 9, 20, "jump if zero", 0, 1, ft_zjmp},
	{"ldi", 3, {T_REG | T_DIR | T_IND, T_DIR | T_REG, T_REG}, 10, 25,
		"load index", 1, 1, ft_ldi_lldi},
	{"sti", 3, {T_REG, T_REG | T_DIR | T_IND, T_DIR | T_REG}, 11, 25,
		"store index", 1, 1, ft_sti},
	{"fork", 1, {T_DIR}, 12, 800, "fork", 0, 1, ft_fork_lfork},
	{"lld", 2, {T_DIR | T_IND, T_REG}, 13, 10, "long load", 1, 0, ft_ld_lld},
	{"lldi", 3, {T_REG | T_DIR | T_IND, T_DIR | T_REG, T_REG}, 14, 50,
		"long load index", 1, 1, ft_ldi_lldi},
	{"lfork", 1, {T_DIR}, 15, 1000, "long fork", 0, 1, ft_fork_lfork},
	{"aff", 1, {T_REG}, 16, 2, "aff", 1, 0, ft_aff}
};

/*
**	g_params for storing sizes of arg types
**	easy access - g_params[dirsize flag][REG_CODE] = T_REG and so on
*/

unsigned char g_params[2][4] = {{0, T_REG, T_DIR, T_IND}, {0, T_REG, 2, T_IND}};

void		ft_init_arena(t_arena *arena)
{
	arena->carriages_nbr = 0;
	arena->carriage_index = 0;
	arena->carriages = NULL;
	arena->dump_nbr_cycles = NULL;
	arena->players = NULL;
	arena->cycles_to_die = CYCLE_TO_DIE;
	arena->cycle_change_cycles_to_die = 0;
	arena->live_nbr = 0;
	arena->nbr_cycles = 0;
	arena->checks_nbr = 0;
	arena->verbose = 1;
	arena->visual = NULL;
	arena->v = 0;
	ft_bzero(arena->core, MEM_SIZE);
}

void		ft_read_args(t_arena *arena, int argc, char **argv)
{
	int i;

	i = 0;
	while (++i < argc)
	{
		if (ft_strequ(argv[i], "-n"))
		{
			ft_check_next_args(i, argc, argv, 2);
			ft_read_champion(&(argv[i + 1]), &(argv[i + 2]), arena);
			i += 2;
		}
		else if (ft_strequ(argv[i], "-dump"))
		{
			ft_check_next_args(i, argc, argv, 1);
			ft_set_dump(argv[++i], arena);
		}
		else if (ft_strequ(argv[i], "-v"))
		{
			ft_check_next_args(i, argc, argv, 1);
			arena->verbose = ft_atoi(argv[++i]);
		}
		else
			ft_read_champion(NULL, &(argv[i]), arena);
	}
}

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
		ft_reverse_bytes(((t_player*)players->content)->header.prog_size));
		pc += MEM_SIZE / arena->carriages_nbr;
		players = players->next;
	}
	arena->live_id = -ft_reverse_bytes(carriage->regs[1]);
	carriage->arena->carriage_index = carriage->arena->carriages_nbr;
}

int			main(int argc, char **argv)
{
	t_arena arena;

	if (argc < 2)
		ft_exit("\
		usage: ./corewar [-dump N -v N] [[-n N] champion1.cor] ...\n\
		-dump N	: Dumps memory after N cycles then exits\n\
		-v N : Verbosity levels, can be added together to enable several\n\
		- 0 : Show only essentials\n\
		- 1 : Show lives (default)\n\
		- 2 : Show cycles\n\
		- 4 : Show operations (Params are NOT litteral ...)\n\
		- 8 : Show deaths\n\
		-n N : Set unic champion number", "");
	ft_init_arena(&arena);
	ft_read_args(&arena, argc, argv);
	ft_put_players_to_arena(&arena);
	arena.v = 1;
	arena.verbose = 0;
	if (arena.v)
		visualize(&arena);
	else
		ft_start_game(&arena);
	ft_cleanup(&arena);
	return (0);
}
