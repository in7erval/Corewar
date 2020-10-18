
#include "corewar_vs.h"

clock_t	calc_time_delay(t_visual *visual)
{
	return (visual->time + CLOCKS_PER_SEC / visual->cycles_per_sec);
}
