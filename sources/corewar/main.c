/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/08 20:32:21 by majosue           #+#    #+#             */
/*   Updated: 2020/10/27 10:39:24 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

unsigned char g_params[2][4] = {{0, T_REG, T_DIR, T_IND}, {0, T_REG, 2, T_IND}};

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

int			main(int argc, char **argv)
{
	t_arena arena;

	if (argc < 2)
		return (print_usage());
	ft_init_arena(&arena);
	ft_read_args(&arena, argc, argv);
	if (!arena.players)
		return (print_usage());
	ft_put_players_to_arena(&arena);
	if (arena.v)
		visualize(&arena);
	else
		ft_start_game(&arena);
	ft_cleanup(&arena);
	return (0);
}
