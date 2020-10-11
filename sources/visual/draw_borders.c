
#include "corewar_vs.h"

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
