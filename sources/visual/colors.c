
# include "corewar_vs.h"

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