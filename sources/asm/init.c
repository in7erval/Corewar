/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/26 03:15:33 by majosue           #+#    #+#             */
/*   Updated: 2020/10/26 05:13:50 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

void	print_usage(void)
{
	ft_printf("Usage: vm_champs/asm <sourcefile.s>\n");
	exit(0);
}

t_asm	*init_asm(int fd)
{
	t_asm *assembler;

	assembler = (t_asm *)ft_memalloc(sizeof(t_asm));
	if (!assembler)
		ft_asm_exit(NULL, NULL, NULL, NULL);
	assembler->fd = fd;
	return (assembler);
}
