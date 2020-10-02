/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_tools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/02 05:23:55 by majosue           #+#    #+#             */
/*   Updated: 2020/10/02 05:28:28 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

void	ft_exit(char *str, char *str2)
{
	if (str2)
	{
		ft_putstr_fd(str, 2);
		ft_putendl_fd(str2, 2);
		exit(EXIT_FAILURE);
	}
	perror(str);
	exit(EXIT_FAILURE);
}

void	del(void *memory, size_t size)
{
	ft_bzero(memory, size);
	ft_memdel(&memory);
}

void	del_player(void *memory, size_t size)
{
	ft_bzero(((t_player *)memory)->code, sizeof(t_player));
	ft_memdel(&((t_player *)memory)->code);
	ft_bzero(memory, size);
	ft_memdel(&memory);
}

void	ft_cleanup(t_arena *arena)
{
	ft_lstdel(&arena->carriages, del);
	ft_lstdel(&arena->players, del_player);
	free(arena->dump_nbr_cycles);
}
