
#include "corewar_vs.h"

void 	draw_aff(int c)
{
	mvprintw(36, CORE_WIDTH + 1 + 2, "Aff:");
	mvprintw(36, CORE_WIDTH + 1 + 2 + 4 + 26, "(%d)%c", c, c);
}

clock_t	calc_time_delay(t_visual *visual)
{
	return (visual->time + CLOCKS_PER_SEC / visual->cycles_per_sec);
}
