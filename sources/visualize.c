#include "corewar.h"

void	draw_borders(void)
{
	size_t	i;

	attron(COLOR_PAIR(111));
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
	attroff(COLOR_PAIR(111));
	move(1, 1);
}

void	draw_core(t_visual *visual, t_arena *arena)
{
	size_t i;
	size_t x;
	size_t y;

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
		attron(COLOR_PAIR(visual->map[i] + 100));
		mvprintw(y, x, "%.2x", ((unsigned char *)(arena->core))[i]);
		attroff(COLOR_PAIR(visual->map[i] + 100));
		x += 2;
		i++;
	}
	refresh();
}

void	draw_player(t_player *player)
{
	size_t prog_size;

	prog_size = ft_reverse_bytes(player->header.prog_size);

}

void	draw_players(t_arena *arena)
{
	t_list *players;
	t_player *player;

	players = arena->players;
	while (players) {
		player = (t_player *)players->content;
		draw_player(player);
		players = players->next;
	}
}

void	draw_info(void)
{
	size_t x;
	size_t y;

	x = CORE_WIDTH + 1 + 2;
	y = 2;

	mvprintw(y, x, "SOME USEFUL INFO!");
}

void	init_map_player(t_visual *visual, int offset, int length, int value)
{
	int i;

	i = offset;
	while (i < length + offset)
	{
		visual->map[i] = value;
		i++;
	}
}

void 	init_map(t_arena *arena, t_visual *visual)
{
	size_t id;
	int offset;
	t_list	*players;
	t_player	*player;
	size_t 	value;

	ft_bzero(visual->map, MEM_SIZE);
	id = 1;
	offset = 0;
	value = 1;
	players = arena->players;
	while (id <= arena->players_nbr)
	{
		player = (t_player *)players->content;
		init_map_player(visual, offset, player->prog_size, value);
		id++;
		value++;
		offset += MEM_SIZE / arena->players_nbr;
		players = players->next;
	}
}

void	init_pairs(void)
{
	init_pair(111, COLOR_BLACK, COLOR_WHITE);
	init_pair(100, COLOR_WHITE, COLOR_BLACK);
	init_pair(101, COLOR_GREEN, 0);
	init_pair(102, COLOR_CYAN, 0);
	init_pair(103, COLOR_RED, 0);
	init_pair(104, COLOR_YELLOW, 0);
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

void	visualize(t_arena *arena)
{
	unsigned key;
	t_visual *visual;

	if (!(visual = (t_visual *)ft_memalloc(sizeof(t_visual))))
		ft_exit("ERROR", NULL);
	init_map(arena, visual);
	initscr();
	keypad(stdscr, true);
	nodelay(stdscr, true);
	curs_set(0);
	use_default_colors();
	start_color();

	init_pairs();
	draw_borders();
	draw_core(visual, arena);
	draw_players(arena);
	draw_info();

	while ((key = getch()))
	{
		if (key == KEY_ENTER)
			endwin();
	}
}