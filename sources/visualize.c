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
	if (cycles != (size_t)arena->nbr_cycles && arena->cycles_to_die > 0 && attr->cycles_live > 0)
		attr->cycles_live--;
	if (attr->cycles_live && attr->player)
		return (g_colors[attr->player->nbr + 10] | A_BOLD);
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

void	draw_info(t_arena *arena)
{
	size_t x;
	size_t y;
	t_player *player;
	t_list 	*players;

	x = CORE_WIDTH + 1 + 2;
	y = 2;

	mvprintw(y, x, "Cycle: %d", arena->nbr_cycles);
	y += 2;
	mvprintw(y, x, "Carriages: %d", arena->carriages_nbr);
	players = arena->players;
	while (players)
	{
		y+=2;
		player = (t_player *)players->content;
		mvprintw(y, x, "Player %d - ", player->nbr);
		attron(g_colors[player->nbr]);
		mvprintw(y, x + 11, "%.38s", player->header.prog_name);
		attroff(g_colors[player->nbr]);
		players = players->next;
	}
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
	if (arena->visual->map[carriage->pc].value >= 5 && arena->visual->map[carriage->pc].value <= 9)
		arena->visual->map[carriage->pc].value -= 5;
}

void 	place_carriage(t_arena *arena, t_carriage *carriage)
{
	if (arena->visual->map[carriage->pc].value >= 0 && arena->visual->map[carriage->pc].value <= MAX_PLAYERS)
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

void wait_sec(float sec)
{
	clock_t endwait = clock() + sec * CLOCKS_PER_SEC; // CLOCK_PER_SEC == 1000
	while(clock() < endwait);
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
	if (!(visual = (t_visual *)ft_memalloc(sizeof(t_visual))))
		ft_exit("ERROR", NULL);
	arena->visual = visual;
	init_map(arena);
	draw_carriages(arena);

	while (getch() != ESC)
	{
		play_cycle(arena);
		draw(arena);
		if (getchar() == ESC)
		{
			endwin();
			free(visual);
			break;
		}
	}
}