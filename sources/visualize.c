#include "corewar_vs.h"

void	init_pairs(void)
{
	init_color(COLOR_GRAY, 355, 355, 355);
	init_pair(GRAY, COLOR_GRAY, COLOR_BLACK); //gray
	init_pair(GREEN, COLOR_GREEN, 0);
	init_pair(YELLOW, COLOR_YELLOW, 0);
	init_pair(RED, COLOR_RED, 0);
	init_pair(CYAN, COLOR_CYAN, 0);
	//carriages
	init_pair(GRAY_CARR, COLOR_GRAY, COLOR_WHITE);
	init_pair(GREEN_CARR, COLOR_BLACK, COLOR_GREEN);
	init_pair(CYAN_CARR, COLOR_BLACK, COLOR_CYAN);
	init_pair(RED_CARR, COLOR_BLACK, COLOR_RED);
	init_pair(YELLOW_CARR, COLOR_BLACK, COLOR_YELLOW);
	//live
	init_pair(GREEN_LIVE, COLOR_WHITE, COLOR_GREEN);
	init_pair(CYAN_LIVE, COLOR_WHITE, COLOR_CYAN);
	init_pair(RED_LIVE, COLOR_WHITE, COLOR_RED);
	init_pair(YELLOW_LIVE, COLOR_WHITE, COLOR_YELLOW);
	init_pair(COMMON, COLOR_WHITE, COLOR_BLACK);
	init_pair(BORDER, COLOR_BLACK, COLOR_WHITE);
}

void	draw_borders(void)
{
	size_t	i;

	attron(COLOR_PAIR(BORDER));
	i = 0;
	while (i < WIN_WIDTH)
	{
		mvaddch(0, i, ' ');
		mvaddch(WIN_HEIGHT, i, ' ');
		i++;
	}
	i = 0;
	while (i < WIN_HEIGHT)
	{
		mvaddch(i, 0, ' ');
		mvaddch(i, CORE_WIDTH, ' ');
		mvaddch(i, WIN_WIDTH, ' ');
		i++;
	}
	mvaddch(WIN_HEIGHT, WIN_WIDTH, ' ');
	attroff(COLOR_PAIR(BORDER));
	move(1, 1);
}

int	get_attribute(t_arena *arena, t_attr *attr, size_t cycles)
{
	if (cycles != (size_t)arena->nbr_cycles &&
		arena->cycles_to_die > 0
		&& attr->cycles_live > 0)
		attr->cycles_live--;
	if (cycles != (size_t)arena->nbr_cycles &&
		arena->cycles_to_die > 0 &&
		attr->cycles_store > 0)
		attr->cycles_store--;
	if (attr->cycles_live && attr->player)
		return (g_colors[attr->player->nbr + 10] | A_BOLD);
	else if (attr->cycles_store)
		return (g_colors[attr->value] | A_BOLD);
	else
		return (g_colors[attr->value]);
}

void	draw_core(t_arena *arena)
{
	static size_t	cycles = 0;
	size_t			i;
	size_t 			x;
	size_t			y;
	int				attr;

	i = 0;
	x = 2;
	y = 2;
	while (i < MEM_SIZE)
	{
		if (i / DUMP_WIDTH && !(i % DUMP_WIDTH))
		{
			y++;
			x = 2;
		}
		x++;
		attr = get_attribute(arena, &(arena->visual->map[i]), cycles);
		attron(attr);
		mvprintw(y, x, "%.2x", ((unsigned char *)(arena->core))[i]);
		attroff(attr);
		x += 2;
		i++;
	}
	cycles = arena->nbr_cycles;
}

void 	draw_addition_info(t_arena *arena, int y, int x)
{
	mvprintw(y, x, "CYCLE_TO_DYE: %d", arena->cycles_to_die);
	mvprintw(y + 2, x, "CYCLE_DELTA: %d", CYCLE_DELTA);
	mvprintw(y + 4, x, "Lives: %.6d/%d", arena->live_nbr, NBR_LIVE);
	mvprintw(y + 6, x, "Checks: %.6d/%d", arena->checks_nbr, MAX_CHECKS);
}

void 	draw_players_info(t_arena *arena, int y, int x)
{
	t_player *player;
	t_list 	*players;

	players = arena->players;
	while (players)
	{
		y += 2;
		player = (t_player *)players->content;
		mvprintw(y, x, "Player -%d:", player->nbr);
		attron(g_colors[player->nbr]);
		mvprintw(y, x + 11, "%.38s", player->header.prog_name);
		attroff(g_colors[player->nbr]);
		players = players->next;
		mvprintw(++y, x + 2, "Last live: %30d", player->last_live);
		mvprintw(++y, x + 2, "Lives in current period: %16d", player->current_lives);
	}
	draw_addition_info(arena, y + 2, x);
}

void 	draw_winner(t_arena *arena)
{
	t_player *player;

	player = ft_get_player(arena, arena->live_id);
	if (player)
	{
		mvprintw(50, CORE_WIDTH + 1 + 2, "Contestant %d, \"", arena->live_id);
		attron(g_colors[player->nbr]);
		printw("%s", player->header.prog_name);
		attroff(g_colors[player->nbr]);
		printw("\", has won !");
	}
}

void 	draw_help(void)
{
	mvprintw(40, CORE_WIDTH + 3, "Controls:");
	mvprintw(41, CORE_WIDTH + 3, "'q' - -10 to cycles_per_sec");
	mvprintw(42, CORE_WIDTH + 3, "'w' - -1 to cycles_per_sec");
	mvprintw(43, CORE_WIDTH + 3, "'e' - +1 to cycles_per_sec");
	mvprintw(44, CORE_WIDTH + 3, "'r' - +10 to cycles_per_sec");
	mvprintw(45, CORE_WIDTH + 3, "'d' - debug mode (press Space)");
}

void	draw_info(t_arena *arena)
{
	size_t x;
	size_t y;

	x = CORE_WIDTH + 1 + 2;
	y = 2;
	attron(A_BOLD);
	attron(g_colors[(arena->visual->is_running || arena->visual->debug) ? 1 : 3]);
	mvprintw(y, x, (arena->visual->is_running || arena->visual->debug) ? "** Running **" : "** Stopped **");
	attroff(g_colors[(arena->visual->is_running || arena->visual->debug) ? 1 : 3]);
	mvprintw(y + 2, x, "Cycles_per_sec: %d", arena->visual->cycles_per_sec);
	mvprintw(y + 4, x, "Cycle: %d", arena->nbr_cycles);
	mvprintw(y + 6, x, "Carriages: %d", arena->carriages_nbr);
	draw_players_info(arena, y + 8, x);
	draw_help();
	if (!arena->carriages_nbr)
		draw_winner(arena);
	attroff(A_BOLD);
}

void	init_map_player(t_visual *visual, size_t offset, t_player *player)
{
	size_t i;
	size_t len;

	len = ft_reverse_bytes(player->header.prog_size);
	i = offset;
	while (i < len + offset)
	{
		visual->map[i].value = (player->nbr - 1) % MAX_PLAYERS + 1;
		i++;
	}
}

void ft_fill_space(t_attr *space, size_t size, size_t c)
{
	size_t i;

	i = 0;
	while (i < size)
	{
		space[i].player = NULL;
		space[i].cycles_live = 0;
		space[i].cycles_store = 0;
		space[i].value = c;
		i++;
	}
}

void 	init_map(t_arena *arena)
{
	size_t	offset;
	t_list		*players;
	t_player	*player;

	//ft_fill_space(arena->visual->map, MEM_SIZE, 10);
	offset = 0;
	players = arena->players;
	while (players)
	{
		player = (t_player *)players->content;
		init_map_player(arena->visual, offset, player);
		offset += MEM_SIZE / arena->carriages_nbr;
		players = players->next;
	}
}

void	print_map(t_visual *visual)
{
	size_t i;

	i = 0;
	while (i < MEM_SIZE)
	{
		if (i != 0 && i % 64 == 0)
			ft_printf("\n");
		ft_printf("%d ", visual->map[i]);
		i++;
	}
}

void 	remove_carriage(t_arena *arena, t_carriage *carriage)
{
	if (arena->visual->map[carriage->pc].value >= 5 &&
		arena->visual->map[carriage->pc].value <= 9)
		arena->visual->map[carriage->pc].value -= 5;
}

void 	place_carriage(t_arena *arena, t_carriage *carriage)
{
	if (arena->visual->map[carriage->pc].value >= 0 &&
		arena->visual->map[carriage->pc].value <= MAX_PLAYERS)
		arena->visual->map[carriage->pc].value += 5;
}

void	draw_carriages(t_arena *arena)
{
	t_carriage	*carriage;
	t_list		*carriages;

	carriages = arena->carriages;
	while (carriages)
	{
		carriage = (t_carriage *)carriages->content;
		place_carriage(arena, carriage);
		carriages = carriages->next;
	}
}

void 	draw(t_arena *arena)
{
	erase();
	draw_borders();
	draw_info(arena);
	draw_core(arena);
	refresh();
}

void 	play_cycle(t_arena *arena)
{
	if (arena->carriages_nbr)
	{
		arena->nbr_cycles++;
		ft_lstiter(arena->carriages, ft_run_carriages);
		ft_check_arena(arena);
	}
}

void 	update_map(t_arena *arena, t_carriage *carriage, int addr, int size)
{
	int s;

	while (size)
	{
 		s = (addr + size - 1) % MEM_SIZE;
 		if (s < 0)
 			s += MEM_SIZE;
		arena->visual->map[s].value = carriage->owner->nbr;
		arena->visual->map[s].cycles_store = 50;
		size--;
	}
}

void 	handle_keyboard(t_arena *arena)
{
	if (arena->visual->button == 'd')
		arena->visual->debug = !arena->visual->debug;
	else if (arena->visual->button == 'w')
		arena->visual->cycles_per_sec--;
	else if (arena->visual->button == 'q')
		arena->visual->cycles_per_sec -= 10;
	else if (arena->visual->button == 'e')
		arena->visual->cycles_per_sec++;
	else if (arena->visual->button == 'r')
		arena->visual->cycles_per_sec += 10;
	else if (arena->visual->button == ' ')
		arena->visual->is_running = !arena->visual->is_running;
	if (arena->visual->cycles_per_sec > 1000)
		arena->visual->cycles_per_sec = 1000;
	if (arena->visual->cycles_per_sec <= 0)
		arena->visual->cycles_per_sec = 1;
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
	arena->visual = visual;
	return (visual);
}

clock_t	calc_time_delay(t_visual *visual)
{
	return (visual->time + CLOCKS_PER_SEC / visual->cycles_per_sec);
}

void	visualize(t_arena *arena)
{
	t_visual *visual;

	initscr();
	keypad(stdscr, true);
	nodelay(stdscr, true);
	curs_set(0);
	cbreak();
	use_default_colors();
	start_color();
	init_pairs();
	visual = init_visual(arena);
	init_map(arena);
	draw_carriages(arena);
	while ((arena->visual->button = getch()) != ESC)
	{
//		while (arena->nbr_cycles < 20000)
//			play_cycle(arena);
		handle_keyboard(arena);
		if (arena->visual->is_running && !arena->visual->debug && (clock() >= calc_time_delay(arena->visual)))
		{
			arena->visual->time = clock();
			play_cycle(arena);
		}
		else if (arena->visual->button == ' ' && arena->visual->debug)
			play_cycle(arena);
		draw(arena);
	}
	endwin();
	free(visual);
}
