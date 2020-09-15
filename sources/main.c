/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/08 20:32:21 by majosue           #+#    #+#             */
/*   Updated: 2020/09/15 20:19:33 by majosue          ###   ########.fr       */
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
	arena->players = NULL;
	arena->cycles_to_die = CYCLE_TO_DIE;
	arena->live_nbr = 0;
	arena->nbr_cycles = 0;
	arena->checks_nbr = 0;

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

int ft_find_next_avialable_number(t_list *players, int delta)
{
	int nbr;
	int is_unic;
	t_list *head;

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

int ft_read_player_number(char **str, t_arena *arena)
{
	long number;
//	t_list *carriages;
	t_list *players;
	
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
//	t_carriage *carrage;
//	t_list *new_carrage;
	t_player *player;
	t_list	*new_player;

	if (!(player = (t_player *)ft_memalloc(sizeof(*player))))
		ft_exit("ERROR", NULL);
	player->nbr = ft_read_player_number(str, arena);
	player->header = ft_read_header(*file, &fd);
	player->code = ft_read_code(&fd, ft_reverse_bytes(player->header.prog_size), *file);
	//ft_bzero(&carrage->regs, REG_SIZE * REG_NUMBER);
	//carrage->carry = 1;
	//carrage->regs[0] = -carrage->player_nbr; // по методичке Бражника так но почему? Зачем минус нужен?
	if (!(new_player = ft_lstnew(player, sizeof(*player))))
		ft_exit("ERROR", NULL);
	ft_lstadd(&(arena->players), new_player);
	if (++arena->carriages_nbr > MAX_PLAYERS)
		ft_exit("ERROR: exeded players maximum", "");
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
	t_carriage *carriage;
	t_list *players;
	t_list *new_carriage;
	int pc;

	pc = 0;
	players = arena->players;
	while(players)
	{
		if(!(carriage = (t_carriage*)ft_memalloc(sizeof(*carriage))))
			ft_exit("ERROR", NULL);
		carriage->cycles_to_die = &arena->cycles_to_die;
		carriage->nbr_cycles = &arena->nbr_cycles;
		carriage->carriages_nbr = &arena->carriages_nbr;
		carriage->pc = pc;
		carriage->regs[0] = ((t_player*)players->content)->nbr; 
		if (!(new_carriage = (ft_lstnew(carriage, sizeof(*carriage)))))
			ft_exit("ERROR", NULL);
		ft_lstadd_back(&arena->carriages, new_carriage);
		ft_memmove(arena->core + pc, ((t_player*)players->content)->code,
		ft_reverse_bytes(((t_player*)players->content)->header.prog_size));
		pc += MEM_SIZE / arena->carriages_nbr;
		players = players->next;
	}
	arena->live_id = ((t_carriage*)arena->carriages->content)->regs[0];
}

/*
**	Get player with id
**	Return NULL if player not exist
*/

t_player	*ft_get_player(t_arena *arena, int id)
{
	t_list *players;

	players = arena->players;
	while (players && id != ((t_player *)players->content)->nbr)
		players = players->next;
	if (players)
		return (((t_player *)players->content));
	return NULL;
}

void ft_mark_death_carriages(t_list *carriages)
{
	t_carriage *carriage;

	carriage = ((t_carriage *)(carriages->content));
	if (carriage->death == 0 && (*(carriage->nbr_cycles) - carriage->last_live_cycle) < *(carriage->cycles_to_die))
		{
			carriage->death = 1;
			(*(carriage->carriages_nbr))--;
		}
}

int ft_check_arena(t_arena *arena)
{
	if (arena->cycles_to_die > 0 && ((arena->nbr_cycles + 1) % arena->cycles_to_die)) 
		return(0);
	ft_printf("Cycle #%d check\n", arena->nbr_cycles);
	ft_lstiter(arena->carriages, ft_mark_death_carriages);	
	if (arena->carriages_nbr == 0)
		return(1);
	if (arena->live_nbr >= NBR_LIVE || arena->checks_nbr >= MAX_CHECKS)
	{
		arena->cycles_to_die -= CYCLE_DELTA;
		arena->checks_nbr = 0;
	}
	else 
		arena->checks_nbr++;
	arena->live_nbr = 0;
	return (0);
}

void ft_start_game(t_arena *arena)
{
	while(1)
	{
		if (arena->dump_nbr_cycles && *(arena->dump_nbr_cycles) == arena->nbr_cycles)
		{
			ft_print_memory(arena->core, MEM_SIZE);
			break;
		}
	 //	ft_run_carriages(arena);
		if (ft_check_arena(arena))
			{
				ft_printf("Player %d (%s) won\n", arena->live_id, ft_get_player(arena, arena->live_id)->header.prog_name);
				break;
			}
		(arena->nbr_cycles)++;
	}	
}

int main(int argc, char **argv)
{
	t_arena arena;

	if (argc < 2)
		ft_exit("usage: ./corewar [-dump nbr_cycles] [[-n number] champion1.cor] ...", "");
	ft_init_arena(&arena);
	ft_read_args(&arena, argc, argv);
	ft_put_players_to_arena(&arena);
	ft_start_game(&arena);
	//ft_cleanup(&arena);
	return (0);
}
