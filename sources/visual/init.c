/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htrent <htrent@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/30 15:31:44 by htrent            #+#    #+#             */
/*   Updated: 2020/10/30 15:31:45 by htrent           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar_vs.h"

void		init_map_player(t_visual *visual, size_t offset, t_player *player,
							int nbr)
{
	size_t	i;
	size_t	len;

	len = ft_reverse_bytes(player->header.prog_size);
	i = offset;
	while (i < len + offset)
	{
		player->nbr_color = nbr;
		visual->map[i].value = (nbr - 1) % MAX_PLAYERS + 1;
		i++;
	}
}

void		init_map(t_arena *arena)
{
	size_t		offset;
	t_list		*players;
	t_player	*player;
	int			nbr;

	offset = 0;
	nbr = 1;
	players = arena->players;
	while (players)
	{
		player = (t_player *)players->content;
		init_map_player(arena->visual, offset, player, nbr);
		offset += MEM_SIZE / arena->carriages_nbr;
		nbr++;
		players = players->next;
	}
}

void		init_pairs(void)
{
	init_color(COLOR_GRAY, 355, 355, 355);
	init_pair(GRAY, COLOR_GRAY, COLOR_BLACK);
	init_pair(GREEN, COLOR_GREEN, 0);
	init_pair(YELLOW, COLOR_YELLOW, 0);
	init_pair(RED, COLOR_RED, 0);
	init_pair(CYAN, COLOR_CYAN, 0);
	init_pair(GRAY_CARR, COLOR_GRAY, COLOR_WHITE);
	init_pair(GREEN_CARR, COLOR_BLACK, COLOR_GREEN);
	init_pair(CYAN_CARR, COLOR_BLACK, COLOR_CYAN);
	init_pair(RED_CARR, COLOR_BLACK, COLOR_RED);
	init_pair(YELLOW_CARR, COLOR_BLACK, COLOR_YELLOW);
	init_pair(GREEN_LIVE, COLOR_WHITE, COLOR_GREEN);
	init_pair(CYAN_LIVE, COLOR_WHITE, COLOR_CYAN);
	init_pair(RED_LIVE, COLOR_WHITE, COLOR_RED);
	init_pair(YELLOW_LIVE, COLOR_WHITE, COLOR_YELLOW);
	init_pair(COMMON, COLOR_WHITE, COLOR_BLACK);
	init_pair(BORDER, COLOR_BLACK, COLOR_WHITE);
}

t_visual	*init_visual(t_arena *arena)
{
	t_visual *visual;

	if (!(visual = (t_visual *)ft_memalloc(sizeof(t_visual))))
		ft_exit("ERROR", NULL);
	visual->time = 0;
	visual->cycles_per_sec = 50;
	visual->debug = 0;
	visual->is_running = 0;
	visual->has_aff = 0;
	arena->visual = visual;
	return (visual);
}

void		init_window(void)
{
	initscr();
	keypad(stdscr, true);
	nodelay(stdscr, true);
	curs_set(0);
	cbreak();
	use_default_colors();
	start_color();
	init_pairs();
}
