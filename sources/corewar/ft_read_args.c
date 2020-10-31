/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_read_args.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/02 04:37:41 by majosue           #+#    #+#             */
/*   Updated: 2020/10/02 05:35:49 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

int			ft_read_player_number(char **str, t_arena *arena)
{
	long	number;
	t_list	*players;

	number = 1;
	players = arena->players;
	if (!str && !players)
		return (number);
	else if (!str && players)
		number = ft_find_next_avialable_number(arena->players, 1);
	else
	{
		number = ft_atoi(*str);
		while (players)
		{
			if (((t_player *)(players->content))->nbr == number)
				ft_exit("ERROR: already used player number - ", *str);
			players = players->next;
		}
	}
	if (number <= 0 || number > INT32_MAX)
		ft_exit("ERROR: wrong player number", "");
	return (number);
}

t_header	ft_read_header(char *file, int *fd)
{
	t_header header;

	if ((*fd = open(file, O_RDONLY)) < 0)
		ft_exit("ERROR", NULL);
	if (read(*fd, &header, sizeof(header)) != sizeof(header))
		ft_exit("ERROR: reading header in file ", file);
	if (ft_reverse_bytes(header.magic) != COREWAR_EXEC_MAGIC)
		ft_exit("ERROR: magic in header in file ", file);
	if (header.prog_name[PROG_NAME_LENGTH] != 0)
		ft_exit("ERROR: no string terminator found in champion name in ", file);
	if (header.comment[COMMENT_LENGTH] != 0)
		ft_exit("ERROR: no string terminator found in comments in ", file);
	if (ft_reverse_bytes(header.prog_size) > CHAMP_MAX_SIZE)
		ft_exit("ERROR: champion prog_size too big in ", file);
	if (ft_reverse_bytes(header.prog_size) <= 0)
		ft_exit("ERROR: no champion prog found in ", file);
	return (header);
}

void		*ft_read_code(int *fd, int size, char *file)
{
	void	*code;
	off_t	old_position;
	off_t	end_position;

	if (!(code = ft_memalloc(size)))
		ft_exit("ERROR", NULL);
	if (read(*fd, code, size) != size)
		ft_exit("ERROR reading champion programm in ", file);
	old_position = lseek(*fd, 0, SEEK_CUR);
	end_position = lseek(*fd, 0, SEEK_END);
	if (old_position != end_position)
	{
		close(*fd);
		ft_exit("ERROR: some data found after champion programm in ", file);
	}
	close(*fd);
	return (code);
}

void		ft_read_champion(char **str, char **file, t_arena *arena)
{
	int			fd;
	t_player	*player;
	t_list		*new_player;

	if (!(player = (t_player *)ft_memalloc(sizeof(*player))))
		ft_exit("ERROR", NULL);
	player->nbr = ft_read_player_number(str, arena);
	player->header = ft_read_header(*file, &fd);
	player->code = ft_read_code(&fd,
	ft_reverse_bytes(player->header.prog_size), *file);
	player->current_lives = 0;
	player->last_live = 0;
	if (!(new_player = ft_lstnew(player, sizeof(*player))))
		ft_exit("ERROR", NULL);
	ft_lstadd_back(&(arena->players), new_player);
	if (++arena->carriages_nbr > MAX_PLAYERS)
		ft_exit("ERROR: exeded players maximum", "");
}

void		ft_set_dump(char *str, t_arena *arena)
{
	arena->dump_nbr_cycles = ft_atoi(str);
	if (arena->dump_nbr_cycles < 0)
		ft_exit("ERROR: nbr_cycles wrong ", "");
}
