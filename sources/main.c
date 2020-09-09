/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/08 20:32:21 by majosue           #+#    #+#             */
/*   Updated: 2020/09/09 15:07:18 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

void	ft_exit(char *str, char *str2)
{
	if (str)
	{
		ft_putstr_fd(str, 2);
		ft_putendl_fd(str2, 2);
		exit(EXIT_FAILURE);
	}
	perror(NULL);
	exit(EXIT_FAILURE);
}

unsigned int reverse_bytes(unsigned int value)
{
     return (value & 0x000000FFU) << 24 | (value & 0x0000FF00U) << 8 |
         (value & 0x00FF0000U) >> 8 | (value & 0xFF000000U) >> 24;
}

int main(int argc, char **argv) 
{
	int			fd;
	t_header	header;
	void		*code;

	if (argc < 2)
        ft_exit("usage: ./corewar [-dump nbr_cycles] [[-n number] champion1.cor] ...", "");
	if ((fd = open(argv[1], O_RDONLY)) < 0)
        ft_exit(NULL, NULL);

/* read from file */
	if (read(fd, &header, sizeof(header)) < 0)
        ft_exit("Error reading file ", argv[1]);
	code = ft_memalloc(reverse_bytes(header.prog_size));
	read(fd, code, reverse_bytes(header.prog_size));
	ft_printf("magic = %x\nprog_name = %s\nprog_size = %d\ncomment = %s\n", reverse_bytes(header.magic), header.prog_name, reverse_bytes(header.prog_size), header.comment);
	close(fd);

/* write to another file */
	if ((fd = open("for_compare.cor", O_WRONLY | O_CREAT, 0644)) < 0)
        ft_exit(NULL, NULL);
	write(fd, &header, sizeof(header));
	write(fd, code, reverse_bytes(header.prog_size));
	close(fd);
	ft_memdel(&code);
/* result file is copy of original */
	return (0);
}
