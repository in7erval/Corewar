/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove_circle.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/02 02:21:08 by majosue           #+#    #+#             */
/*   Updated: 2020/10/02 05:38:52 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

void	ft_print_memory(void *mem, size_t size)
{
	size_t i;
	size_t ofset;

	i = 0;
	ofset = 0;
	while (i < size)
	{
		if (i / DUMP_WIDTH && !(i % DUMP_WIDTH))
			ft_printf("\n");
		if (!(i % DUMP_WIDTH))
		{
			ft_printf("0x%.4x :", (int)ofset);
			ofset += DUMP_WIDTH;
		}
		ft_printf(" ");
		ft_printf("%.2x", ((unsigned char*)(mem))[i]);
		i++;
	}
	ft_printf("\n");
}

void	ft_memmove_from_core_to_core(void *dst, void *src,
t_carriage *carriage, size_t size)
{
	size_t dst_idx;
	size_t src_idx;
	size_t i;

	dst_idx = (unsigned char *)dst - carriage->core;
	src_idx = (unsigned char *)src - carriage->core;
	i = 0;
	while (i < size)
	{
		carriage->core[(dst_idx + i) % MEM_SIZE] =
		carriage->core[(src_idx + i) % MEM_SIZE];
		i++;
	}
}

void	ft_memmove_from_core(void *dst, void *src,
t_carriage *carriage, size_t size)
{
	size_t src_idx;
	size_t i;

	i = 0;
	src_idx = (unsigned char *)src - carriage->core;
	while (i < size)
	{
		((unsigned char *)dst)[i] = carriage->core[(src_idx + i) % MEM_SIZE];
		i++;
	}
}

void	ft_memmove_to_core(void *dst, void *src,
t_carriage *carriage, size_t size)
{
	size_t dst_idx;
	size_t i;

	i = 0;
	dst_idx = (unsigned char *)dst - carriage->core;
	while (i < size)
	{
		carriage->core[(dst_idx + i) % MEM_SIZE] = ((unsigned char *)src)[i];
		i++;
	}
}

void	ft_memmove_circle(void *dst, void *src,
t_carriage *carriage, size_t size)
{
	long dst_idx;
	long src_idx;

	dst_idx = (unsigned char *)dst - carriage->core;
	src_idx = (unsigned char *)src - carriage->core;
	if (dst_idx >= 0 && dst_idx < MEM_SIZE &&
	src_idx >= 0 && src_idx < MEM_SIZE)
		ft_memmove_from_core_to_core(dst, src, carriage, size);
	else if (dst_idx >= 0 && dst_idx < MEM_SIZE)
		ft_memmove_to_core(dst, src, carriage, size);
	else if (src_idx >= 0 && src_idx < MEM_SIZE)
		ft_memmove_from_core(dst, src, carriage, size);
	else
		ft_memmove(dst, src, size);
}
