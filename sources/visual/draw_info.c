/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_info.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htrent <htrent@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/30 15:31:31 by htrent            #+#    #+#             */
/*   Updated: 2020/10/30 15:31:34 by htrent           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar_vs.h"

void	draw_addition_info(t_arena *arena)
{
	int x;

	x = CORE_WIDTH + 1 + 2;
	mvprintw(28, x, "%-32s %d", "CYCLE_TO_DYE:", arena->cycles_to_die);
	mvprintw(30, x, "%-32s %d", "CYCLE_DELTA:", CYCLE_DELTA);
	mvprintw(32, x, "%-32s %.4d/%d", "Lives:", arena->live_nbr, NBR_LIVE);
	mvprintw(34, x, "%-32s %.2d/%d", "Checks:", arena->checks_nbr > 0 ?
		arena->checks_nbr - 1 : 0, MAX_CHECKS);
	mvprintw(36, x, "%-32s %d", "Cycles_to_checks:", (arena->cycles_to_die > 0)
					? arena->cycles_to_die - arena->after_check : 0);
	if (arena->visual->has_aff)
		mvprintw(38, x, "%-32s (%d)%c", "Aff:", arena->visual->aff,
		arena->visual->aff);
}

void	draw_players_info(t_arena *arena, int y, int x)
{
	t_player	*player;
	t_list		*players;

	players = arena->players;
	while (players)
	{
		y += 2;
		player = (t_player *)players->content;
		mvprintw(y, x, "Player -%d : ", player->nbr);
		attron(g_colors[player->nbr_color]);
		printw("%.38s", player->header.prog_name);
		attroff(g_colors[player->nbr_color]);
		players = players->next;
		mvprintw(++y, x + 2, "Last live: %30d", player->last_live);
		mvprintw(++y, x + 2, "Lives in current period: %16d",
			player->current_lives);
	}
}

void	draw_help(void)
{
	mvprintw(59, CORE_WIDTH + 3, "CONTROLS:");
	mvprintw(61, CORE_WIDTH + 3, "'q' -- -10 to cycles_per_sec");
	mvprintw(62, CORE_WIDTH + 3, "'w' -- -1 to cycles_per_sec");
	mvprintw(63, CORE_WIDTH + 3, "'e' -- +1 to cycles_per_sec");
	mvprintw(64, CORE_WIDTH + 3, "'r' -- +10 to cycles_per_sec");
	mvprintw(65, CORE_WIDTH + 3, "'d' -- debug mode (press Space)");
}

void	draw_winner(t_arena *arena)
{
	t_player *player;

	player = ft_get_player(arena, arena->live_id);
	if (player)
	{
		mvprintw(40, CORE_WIDTH + 1 + 2, "Contestant %d, \"", arena->live_id);
		attron(g_colors[player->nbr_color]);
		printw("%.22s", player->header.prog_name);
		attroff(g_colors[player->nbr_color]);
		printw("\", has won !");
	}
}

void	draw_info(t_arena *arena)
{
	size_t x;
	size_t y;

	x = CORE_WIDTH + 1 + 2;
	y = 2;
	attron(A_BOLD);
	attron(g_colors[(arena->visual->is_running || arena->visual->debug) ?
						1 : 3]);
	mvprintw(y, x, (arena->visual->is_running || arena->visual->debug) ?
					"** Running **" : "** Stopped **");
	attroff(g_colors[(arena->visual->is_running || arena->visual->debug) ?
						1 : 3]);
	mvprintw(y + 2, x, "Cycles_per_sec: %d", arena->visual->cycles_per_sec);
	mvprintw(y + 4, x, "Cycle: %d", arena->nbr_cycles);
	mvprintw(y + 6, x, "Carriages: %d", arena->carriages_nbr);
	draw_players_info(arena, y + 8, x);
	draw_addition_info(arena);
	draw_help();
	if (!arena->carriages_nbr)
		draw_winner(arena);
	attroff(A_BOLD);
}
