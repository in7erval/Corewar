/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_tools.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/02 04:46:14 by majosue           #+#    #+#             */
/*   Updated: 2020/10/02 05:12:19 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

unsigned int	ft_reverse_bytes(unsigned int value)
{
	return (value & 0x000000FFU) << 24 | (value & 0x0000FF00U) << 8 |
	(value & 0x00FF0000U) >> 8 | (value & 0xFF000000U) >> 24;
}

unsigned short	ft_reverse_bytes_short(unsigned short value)
{
	return (value & 0x00FFU) << 8 | (value & 0xFF00U) >> 8;
}

void			ft_check_next_args(int current, int number,
													char **argv, int delta)
{
	if (current + delta >= number)
		ft_exit("ERROR: not enough args after ", argv[current]);
}

int				ft_find_next_avialable_number(t_list *players, int delta)
{
	int		nbr;
	int		is_unic;
	t_list	*head;

	is_unic = 1;
	head = players;
	nbr = ((t_player *)(players->content))->nbr + delta;
	while (players)
	{
		if (((t_player *)(players->content))->nbr == nbr)
			is_unic = 0;
		players = players->next;
	}
	if (!is_unic)
		return (ft_find_next_avialable_number(head, delta + 1));
	return (nbr);
}

/*
**	Get player with id
**	Return NULL if player not exist
*/

t_player		*ft_get_player(t_arena *arena, int id)
{
	t_list *players;

	players = arena->players;
	while (players && id != ((t_player *)players->content)->nbr)
		players = players->next;
	if (players)
		return (((t_player *)players->content));
	return (NULL);
}
