/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_read_params.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htrent <htrent@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/30 15:40:23 by htrent            #+#    #+#             */
/*   Updated: 2020/10/30 15:40:25 by htrent           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

static void	ft_read_args_extension(t_arena *arena, int ac, char **av, int *i)
{
	if (ft_strequ(av[*i], "-n"))
	{
		ft_check_next_args(*i, ac, av, 2);
		ft_read_champion(&(av[*i + 1]), &(av[*i + 2]), arena);
		*i += 2;
	}
	else if (ft_strequ(av[*i], "-dump"))
	{
		ft_check_next_args(*i, ac, av, 1);
		ft_set_dump(av[++(*i)], arena);
	}
	else if (ft_strequ(av[*i], "-verbose"))
	{
		ft_check_next_args(*i, ac, av, 1);
		arena->verbose = ft_atoi(av[++(*i)]);
	}
	else
		ft_read_champion(NULL, &(av[*i]), arena);
}

void		ft_read_args(t_arena *arena, int argc, char **argv)
{
	int i;

	i = 0;
	while (++i < argc)
	{
		if (ft_strequ(argv[i], "-a"))
			arena->a = 1;
		else if (ft_strequ(argv[i], "-v"))
			arena->v = 1;
		else if (ft_strequ(argv[i], "-legacy"))
		{
			ft_check_next_args(i, argc, argv, 1);
			arena->legacy = ft_atoi(argv[++i]);
		}
		else
			ft_read_args_extension(arena, argc, argv, &i);
	}
}
