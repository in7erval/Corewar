/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/08 20:32:21 by majosue           #+#    #+#             */
/*   Updated: 2020/09/10 16:11:57 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

/*
**	Exit wraper 
*/

void ft_exit(char *str, char *str2)
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

unsigned int ft_reverse_bytes(unsigned int value)
{
	return (value & 0x000000FFU) << 24 | (value & 0x0000FF00U) << 8 |
		   (value & 0x00FF0000U) >> 8 | (value & 0xFF000000U) >> 24;
}

void ft_init_arena(t_arena *arena)
{
	arena->carriages_nbr = 0;
	arena->carriages = NULL;
	arena->dump_nbr_cycles = NULL;
	arena->players_nbr = 0;
	arena->players = NULL;
	ft_bzero(arena->core, MEM_SIZE);
}

void ft_check_next_args(int current, int number, char **argv, int delta)
{
	if (current + delta >= number)
		ft_exit("ERROR: not enough args after ", argv[current]);
}

void ft_set_dump(char *str, t_arena *arena)
{
	if (!arena->dump_nbr_cycles)
	{
		if (!(arena->dump_nbr_cycles = ft_memalloc(sizeof(*(arena->dump_nbr_cycles)))))
			ft_exit("ERROR", NULL);
		*(arena->dump_nbr_cycles) = ft_atoi(str);
		if (*(arena->dump_nbr_cycles) < 0)
			ft_exit("ERROR: nbr_cycles wrong ", "");
	}
}

int ft_find_next_available_number(t_list *players, int delta)
{
	int nbr;
	int is_unic;
	t_list *head;
	t_player *player;

	is_unic = 1;
	head = players;
	player = (t_player *)players->content;
	nbr = player->player_nbr + delta;
	while (players)
	{
		player = (t_player *)players->content;
		if (player->player_nbr == nbr)
			is_unic = 0;
		players = players->next;
	}
	if (!is_unic)
		return (ft_find_next_available_number(head, delta + 1));
	return (nbr);
}

int ft_read_player_number(char **str, t_arena *arena)
{
	long number;
	t_list *players;

	number = 1;
	players = arena->players;
	if (!str && !players)
		return (number);
	else if (!str && players)
		number = ft_find_next_available_number(arena->players, 1);
	else
	{
		number = ft_atoi(*str);
		while (players)
		{
			if (((t_player *)(players->content))->player_nbr == number)
				ft_exit("ERROR: already used player number - ", *str);
			players = players->next;
		}
	}
	if (number <= 0 || number > INT32_MAX)
		ft_exit("ERROR: wrong player number", "");
	return (number);
}

t_header ft_read_header(char *file, int *fd)
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
	return (header);
}

void *ft_read_code(int *fd, int size, char *file)
{
	void *code;
	off_t old_position;
	off_t end_position;

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

void ft_read_champion(char **str, char **file, t_arena *arena)
{
	int fd;
	t_carriage *carriage;
	t_list *new_carriage;
	t_player	*player;
	t_list		*new_player;


	if (!(carriage = (t_carriage *)ft_memalloc(sizeof(*carriage))))
		ft_exit("ERROR", NULL);
	if (!(player = (t_player *)ft_memalloc(sizeof(t_player))))
		ft_exit("ERROR", NULL);
	player->player_nbr = ft_read_player_number(str, arena);
	player->header = ft_read_header(*file, &fd);
	player->prog_size = ft_reverse_bytes(player->header.prog_size);
	player->player_code = ft_read_code(&fd, ft_reverse_bytes(player->header.prog_size), *file);
	ft_bzero(&carriage->regs, REG_SIZE * REG_NUMBER);
	carriage->carry = 1;
	carriage->regs[0] = -player->player_nbr;
	carriage->owner = player;
	if (!(new_player = ft_lstnew(player, sizeof(t_player))))
		ft_exit("ERROR", NULL);
	ft_lstadd(&(arena->players), new_player);
	if (!(new_carriage = ft_lstnew(carriage, sizeof(*carriage))))
		ft_exit("ERROR", NULL);
	ft_lstadd(&(arena->carriages), new_carriage);
	if (++arena->carriages_nbr > MAX_PLAYERS)
		ft_exit("ERROR: exeded players maximum", "");
	arena->players_nbr++;
}

void ft_read_args(t_arena *arena, int argc, char **argv)
{
	int i;

	i = 0;
	while (++i < argc)
	{
		if (ft_strequ(argv[i], "-n"))
		{
			ft_check_next_args(i, argc, argv, 2);
			ft_read_champion(&(argv[i + 1]), &(argv[i + 2]), arena);
			i += 2;
		}
		else if (ft_strequ(argv[i], "-dump"))
		{
			ft_check_next_args(i, argc, argv, 1);
			ft_set_dump(argv[++i], arena);
		}
		else
			ft_read_champion(NULL, &(argv[i]), arena);
	}
}

void ft_print_memory(void *mem, size_t size)
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
			ft_printf("0x%.4x:", ofset);
			ofset += DUMP_WIDTH;
		}
		//if (!(i % 2))
			ft_printf(" ");
		ft_printf("%.2x", ((unsigned char*)(mem))[i]);
		i++;
	}
		ft_printf("\n");
}

void ft_put_players_to_arena(t_arena *arena)
{
	int delta;
	unsigned char *ptr;
	t_list *carriages;
	t_carriage *carriage;

	ptr = arena->core;
	carriages = arena->carriages;
	delta = MEM_SIZE / arena->carriages_nbr;
	while(carriages)
	{
		carriage = (t_carriage *)carriages->content;
		carriage->pos = ptr;
		ft_memmove(ptr, carriage->owner->player_code, ft_reverse_bytes(carriage->owner->header.prog_size));
		ptr += delta;
		carriages = carriages->next;
	}
}

int main(int argc, char **argv)
{
	t_arena arena;
	t_list *c;


	if (argc < 2)
		ft_exit("usage: ./corewar [-dump nbr_cycles] [[-n number] champion1.cor] ...", "");
	ft_init_arena(&arena);
	ft_read_args(&arena, argc, argv);
	ft_put_players_to_arena(&arena);
	//ft_start_game(&arena);
	c = arena.players;
	/*
	t_player *player;
	while (c)
	{
		player = (t_player *)c->content;
		ft_printf("player.player_nbr = %d\n", player->player_nbr);
		c = c->next;
	}
	 */
	//ft_print_memory(arena.core, MEM_SIZE);
	visualize(&arena);
	return (0);
}
