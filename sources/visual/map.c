
#include "corewar_vs.h"

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