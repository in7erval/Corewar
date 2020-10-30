/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htrent <htrent@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/30 15:31:38 by htrent            #+#    #+#             */
/*   Updated: 2020/10/30 15:31:40 by htrent           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar_vs.h"

void	handle_keyboard(t_arena *arena)
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
