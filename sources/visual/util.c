/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htrent <htrent@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/30 15:31:54 by htrent            #+#    #+#             */
/*   Updated: 2020/10/30 15:31:56 by htrent           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar_vs.h"

clock_t	calc_time_delay(t_visual *visual)
{
	return (visual->time + CLOCKS_PER_SEC / visual->cycles_per_sec);
}
