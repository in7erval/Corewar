/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/08 20:32:21 by majosue           #+#    #+#             */
/*   Updated: 2020/09/27 14:43:22 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"
#include "op.c" // forbiden by norm?

/*
**	g_params for storing sizes of arg types
**	easy access - g_params[dirsize flag][REG_CODE] = T_REG and so on
*/

unsigned char g_params[2][4] = {{0, T_REG, T_DIR, T_IND}, {0, T_REG, 2, T_IND}};

void ft_memmove_from_core_to_core(void *dst, void *src, t_carriage *carriage, size_t size)
{
	size_t dst_idx;
	size_t src_idx;
	size_t i;

	dst_idx = (unsigned char *)dst - carriage->core;
	src_idx = (unsigned char *)src - carriage->core;
	i = 0;
	while(i < size)
	{
		carriage->core[(dst_idx + i) % MEM_SIZE] = carriage->core[(src_idx + i) % MEM_SIZE];
		i++;
	}
}

void ft_memmove_from_core(void *dst, void *src, t_carriage *carriage, size_t size)
{
	size_t src_idx;
	size_t i;

	i = 0;
	src_idx = (unsigned char *)src - carriage->core;
	while(i < size)
	{
		((unsigned char *)dst)[i] = carriage->core[(src_idx + i) % MEM_SIZE];
		i++;
	}
}

void ft_memmove_to_core(void *dst, void *src, t_carriage *carriage, size_t size)
{
	size_t dst_idx;
	size_t i;

	i = 0;
	dst_idx = (unsigned char *)dst - carriage->core;
	while(i < size)
	{
		carriage->core[(dst_idx + i) % MEM_SIZE] = ((unsigned char *)src)[i];
		i++;
	}
}

void ft_memmove_circle(void *dst, void *src, t_carriage *carriage, size_t size)
{
	long dst_idx;
	long src_idx;

	dst_idx = (unsigned char *)dst - carriage->core;
	src_idx = (unsigned char *)src - carriage->core;
	if (dst_idx >= 0 && dst_idx < MEM_SIZE && src_idx >= 0 && src_idx < MEM_SIZE)
		ft_memmove_from_core_to_core(dst, src, carriage, size);
	else if (dst_idx >= 0 && dst_idx < MEM_SIZE)
		ft_memmove_to_core(dst, src, carriage, size);
	else if (src_idx >= 0 && src_idx < MEM_SIZE)
		ft_memmove_from_core(dst, src, carriage, size);
	else ft_memmove(dst, src, size);
}

/*
**	Move pc to next pos
*/

void ft_skip_args(t_carriage *carriage, int args[3], int addon)
{
	int i;
	int skip;

	i = 0;
	skip = 0;
	while (i < op_tab[carriage->op].max_params)
	{
		skip += g_params[op_tab[carriage->op].short_dir][args[i]];
		i++;
	}
	carriage->pc = (carriage->pc + skip + addon) % MEM_SIZE;
	carriage->wait_cmd = 1;
}

/*
**	Check valid registr number 
**	and store adress of regitr to carriage->params
*/

int	ft_is_valid_regs(t_carriage *carriage, int args[3])
{
	int i;
	int pc;

	i = 0;
	pc = (carriage->pc + 2) % MEM_SIZE;
	while (i < op_tab[carriage->op].max_params)
	{
		if (args[i] == REG_CODE)
		{
			if (carriage->core[pc] <= 0 || carriage->core[pc] > REG_NUMBER)
				return (0);
			carriage->params[i] = &(carriage->regs[carriage->core[pc]]);
		}
			pc = (pc + g_params[op_tab[carriage->op].short_dir][args[i]]) % MEM_SIZE;
		i++;
	}
	return (1);
}

int ft_calculate_pc(t_carriage *carriage, int addon)
{
	int pc;

	addon = addon % MEM_SIZE;
	pc = carriage->pc;
	pc += addon;
	if (pc < 0)
		pc = MEM_SIZE + pc;
	else
		pc = pc % MEM_SIZE;
	return (pc);
}

int	ft_ld_arg_check(t_carriage *carriage, int args[3])
{
	if (!((args[0] == DIR_CODE || args[0] == IND_CODE) &&
	args[1] == REG_CODE && ft_is_valid_regs(carriage, args)))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	ft_st_arg_check(t_carriage *carriage, int args[3])
{
	if (args[0] == REG_CODE  && (args[1] == REG_CODE ||
	args[1] == IND_CODE) && ft_is_valid_regs(carriage, args))
		return (0);
	return (1);
}

int	ft_add_arg_check(t_carriage *carriage, int args[3])
{
if (args[0] == REG_CODE && args[1] == REG_CODE &&
args[2] == REG_CODE && ft_is_valid_regs(carriage, args))
		return (0);
	return (1);
}

int	ft_and_arg_check(t_carriage *carriage, int args[3])
{
if (args[0] != 0 && args[1] != 0 && args[2] == REG_CODE &&
ft_is_valid_regs(carriage, args))
		return (0);
	return (1);
}

int	ft_ldi_arg_check(t_carriage *carriage, int args[3])
{
if (args[0] != 0 && (args[1] == DIR_CODE ||
args[1] == REG_CODE) && args[2] == REG_CODE &&
ft_is_valid_regs(carriage, args))
		return (0);
	return (1);
}

int	ft_sti_arg_check(t_carriage *carriage, int args[3])
{
if (args[0] == REG_CODE && args[1] != 0 &&
(args[2] == DIR_CODE || args[2] == REG_CODE)
&& ft_is_valid_regs(carriage, args))
		return (0);
	return (1);
}

int	ft_sti_aff_check(t_carriage *carriage, int args[3])
{
if (args[0] == REG_CODE && ft_is_valid_regs(carriage, args))
		return (0);
	return (1);
}

void ft_load_params(t_carriage *carriage, int args[3], int mod, int arg_bytes)
{
	int pc;
	int ind_pc;
	int i;
	short ind;
	
	pc = (carriage->pc + 1 + arg_bytes) % MEM_SIZE;
	i = -1;
	while (++i < op_tab[carriage->op].max_params)
	{
		if (args[i] == DIR_CODE)
			carriage->params[i] = &carriage->core[pc];
		if (args[i] == IND_CODE)
		{
			ft_memmove_circle(&ind, &carriage->core[pc],
			carriage, sizeof(short));
			ind = ft_reverse_bytes_short(ind);
			ind = mod ? ind % IDX_MOD : ind;
			ind_pc = ft_calculate_pc(carriage, ind);
			carriage->params[i] = &carriage->core[ind_pc];
		}
		pc = (pc + g_params[op_tab[carriage->op].short_dir][args[i]]) % MEM_SIZE;
	}
}

/*
**	Load values from params adresses to int (live it bigendian)
**	Use DIR_SIZE instead others (we alrady get adreses in load_params)
**	op 0x0d  t_dir = 2 in subject example exec corwar 
*/

void ft_load_values(t_carriage *carriage, int args[3])
{
	int i;
	int size;
	short buffer;
	
	i = 0;
	while (i < op_tab[carriage->op].max_params)
	{
		size = args[i] == IND_CODE || args[i] == REG_CODE ? DIR_SIZE :
		g_params[op_tab[carriage->op].short_dir][args[i]];
		size = args[i] == IND_CODE &&
		op_tab[carriage->op].op_code == 0x0d ? 2 :size;
	if (size < DIR_SIZE)
	{
		ft_memmove_circle(&buffer, carriage->params[i], carriage, size);
		buffer = ft_reverse_bytes_short(buffer);
		carriage->values[i] = buffer;
		carriage->values[i] = ft_reverse_bytes(carriage->values[i]);
	}
	else 
		ft_memmove_circle(&carriage->values[i], carriage->params[i], carriage,
		size);
	i++;
	}
}

int ft_live(t_carriage *carriage, int args[3])
{
	(void)args;
	t_player *player;
	int fake[3];
	int id;
	
	fake[0] = DIR_CODE;
	ft_load_params(carriage, fake, 1, 0);
	ft_load_values(carriage, fake);
	id = ft_reverse_bytes(carriage->values[0]);
	id = -id;
	ft_skip_args(carriage, fake, 1);
	if ((player = ft_get_player(carriage->arena, id)))
	{
		ft_printf("A process shows that player %d (%s) is alive\n",
		player->nbr, player->header.prog_name);
		carriage->arena->live_id = id;
		//return (EXIT_SUCCESS);
	}
	carriage->arena->live_nbr++;
	carriage->last_live_cycle = carriage->arena->nbr_cycles + 1;
	return (EXIT_FAILURE);
}

int ft_ld_lld(t_carriage *carriage, int args[3])
{
	int mod;

	if (ft_ld_arg_check(carriage, args))
	{
		ft_skip_args(carriage, args, 2);
		return (EXIT_FAILURE);
	}
	mod = op_tab[carriage->op].op_code == 2 ? 1 : 0;
	ft_load_params(carriage, args, mod, 1);
	ft_load_values(carriage, args);
	ft_memmove_circle(carriage->params[1],
	&carriage->values[0], carriage, REG_SIZE);
	ft_load_values(carriage, args);
	carriage->carry = carriage->values[1] ? 0 : 1;
	ft_skip_args(carriage, args, 2);
	return (EXIT_SUCCESS);
}

int ft_st(t_carriage *carriage, int args[3])
{
	if (ft_st_arg_check(carriage, args))
	{
		ft_skip_args(carriage, args, 2);
		return (EXIT_FAILURE);
	}
	ft_load_params(carriage, args, 1, 1);
	ft_memmove_circle(carriage->params[1],
	carriage->params[0], carriage, REG_SIZE);
	ft_load_values(carriage, args);
	ft_skip_args(carriage, args, 2);
	return (EXIT_SUCCESS);
}

int ft_add_sub (t_carriage *carriage, int args[3])
{
	int value1;
	int value2;
	int value3;

	if (ft_add_arg_check(carriage, args))
	{
		ft_skip_args(carriage, args, 2);
		return (EXIT_FAILURE);
	}
	ft_memmove(&value1, carriage->params[0], REG_SIZE);
	ft_memmove(&value2, carriage->params[1], REG_SIZE);
	if (op_tab[carriage->op].op_code == 4)
		value3 = ft_reverse_bytes(value1) + ft_reverse_bytes(value2);
	else 
		value3 = ft_reverse_bytes(value1) - ft_reverse_bytes(value2);
	value3 = ft_reverse_bytes(value3);
	ft_memmove(carriage->params[2], &value3, REG_SIZE);
	ft_skip_args(carriage, args, 2);
	carriage->carry = value3 ? 0 : 1;
	return (EXIT_SUCCESS);
}

int ft_and_or_xor(t_carriage *carriage, int args[3])
{
	unsigned int value1;
	unsigned int value2;
	unsigned int value3;

	if (ft_and_arg_check(carriage, args))
	{
		ft_skip_args(carriage, args, 2);
		return (EXIT_FAILURE);
	}
	ft_load_params(carriage, args, 1, 1);
	ft_memmove_circle(&value1, carriage->params[0], carriage, REG_SIZE);
	ft_memmove_circle(&value2, carriage->params[1], carriage, REG_SIZE);
	if (op_tab[carriage->op].op_code == 6)
		value3 = value1 & value2;
	else if (op_tab[carriage->op].op_code == 7)
		value3 = value1 | value2;
	else 
		value3 = value1 ^ value2;	
	ft_memmove_circle(carriage->params[2], &value3, carriage, REG_SIZE);
	ft_skip_args(carriage, args, 2);
	carriage->carry = value3 ? 0 : 1;
	return (EXIT_SUCCESS);
}

int ft_zjmp(t_carriage *carriage, int args[3])
{
	(void)args;
	int fake[3];
	int pc;

	fake[0] = DIR_CODE;
	if (carriage->carry == 0)
	{
			ft_skip_args(carriage, fake, 1);
			return(EXIT_FAILURE);
	}
	ft_load_params(carriage, fake, 1, 0);
	ft_load_values(carriage, fake);
	pc = ft_calculate_pc(carriage, ft_reverse_bytes(carriage->values[0]));
	carriage->pc = pc;
	carriage->wait_cmd = 1;
	return(EXIT_SUCCESS);
}

int ft_ldi_lldi(t_carriage *carriage, int args[3])
{
	int result;
	int pc;

	if (ft_ldi_arg_check(carriage, args))
	{
		ft_skip_args(carriage, args, 2);
		return (EXIT_FAILURE);
	}
	ft_load_params(carriage, args, 1, 1);
	ft_load_values(carriage, args);
	result = ft_reverse_bytes(carriage->values[0]) +
	ft_reverse_bytes(carriage->values[1]);
	result = op_tab[carriage->op].op_code == 10 ? result % IDX_MOD : result;
	pc = ft_calculate_pc(carriage, result);
	ft_memmove_circle(carriage->params[2], &carriage->core[pc],
	carriage, REG_SIZE);
	ft_load_values(carriage, args);
	if (op_tab[carriage->op].op_code == 10)
		carriage->carry = carriage->values[2] ? 1 : 0;
	else
		carriage->carry = carriage->values[2] ? 0 : 1;
	ft_skip_args(carriage, args, 2);
	return (EXIT_SUCCESS);
}

int ft_sti(t_carriage *carriage, int args[3])
{
	int result;
	int pc;

	if (ft_sti_arg_check(carriage, args))
	{
		ft_skip_args(carriage, args, 2);
		return (EXIT_FAILURE);
	}
	ft_load_params(carriage, args, 1, 1);
	ft_load_values(carriage, args);
	result = ft_reverse_bytes(carriage->values[1]) + ft_reverse_bytes(carriage->values[2]);
	result = result % IDX_MOD;
	pc = ft_calculate_pc(carriage, result);
	ft_memmove_circle(&carriage->core[pc], carriage->params[0], carriage, REG_SIZE);
	ft_skip_args(carriage, args, 2);
	return (EXIT_SUCCESS);
}

int ft_fork_lfork(t_carriage *carriage, int args[3])
{
	int fake[3];
	int mod;
	t_carriage 	*new_carriage;
	t_list		*new_node;

	(void)args;
	mod = op_tab[carriage->op].op_code == 12 ? 1 : 0;
	if (!(new_carriage = (t_carriage*)ft_memalloc(sizeof(t_carriage))))
		ft_exit("ERROR", NULL);
	ft_memmove(new_carriage, carriage, sizeof(t_carriage));
	fake[0] = DIR_CODE;
	ft_load_params(carriage, fake, mod, 0);
	ft_load_values(carriage, fake);
	new_carriage->pc = ft_reverse_bytes(carriage->values[0]);
	new_carriage->pc = op_tab[carriage->op].op_code == 12 ? 
	new_carriage->pc % IDX_MOD : new_carriage->pc;
	new_carriage->pc = ft_calculate_pc(carriage, new_carriage->pc);
	if (!(new_node = ft_lstnew(new_carriage, sizeof(*new_carriage))))
		ft_exit("ERROR", NULL);
	ft_lstadd(&carriage->arena->carriages, new_node);
	carriage->arena->carriages_nbr++;
	new_carriage->wait_cmd = 1;
	ft_skip_args(carriage, fake, 1);
	return (EXIT_SUCCESS);
}

int ft_aff(t_carriage *carriage, int args[3])
{
	int c;

	if (!(args[0] == REG_CODE && ft_is_valid_regs(carriage, args)))
		{
			ft_skip_args(carriage, args, 2);
			return(EXIT_FAILURE);
		}
	ft_load_params(carriage, args, 1, 1);
	ft_load_values(carriage, args);
	c = ft_reverse_bytes(carriage->values[0]);
	c = c % 256;
	ft_printf("Aff: %c\n", c);
	ft_skip_args(carriage, args, 2);
	return(EXIT_SUCCESS);
}

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

unsigned short ft_reverse_bytes_short(unsigned short value)
{
	return (value & 0x00FFU) << 8 | (value & 0xFF00U) >> 8;
}

void ft_init_arena(t_arena *arena)
{
	arena->carriages_nbr = 0;
	arena->carriages = NULL;
	arena->dump_nbr_cycles = NULL;
	arena->players = NULL;
	arena->cycles_to_die = CYCLE_TO_DIE;
	arena->cycle_change_cycles_to_die = 0;
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
	t_player *player;
	t_list	*new_player;

	if (!(player = (t_player *)ft_memalloc(sizeof(*player))))
		ft_exit("ERROR", NULL);
	player->nbr = ft_read_player_number(str, arena);
	player->header = ft_read_header(*file, &fd);
	player->code = ft_read_code(&fd, ft_reverse_bytes(player->header.prog_size), *file);
	if (!(new_player = ft_lstnew(player, sizeof(*player))))
		ft_exit("ERROR", NULL);
	ft_lstadd_back(&(arena->players), new_player);
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
			ft_printf("0x%.4x :", (int)ofset);
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
		carriage->core = arena->core;
		carriage->wait_cmd = 1; 
		carriage->pc = pc;
		carriage->arena = arena;
		carriage->regs[1] = ft_reverse_bytes(-((t_player*)players->content)->nbr); 
		if (!(new_carriage = (ft_lstnew(carriage, sizeof(*carriage)))))
			ft_exit("ERROR", NULL);
		ft_lstadd_back(&arena->carriages, new_carriage);
		ft_memmove(arena->core + pc, ((t_player*)players->content)->code,
		ft_reverse_bytes(((t_player*)players->content)->header.prog_size));
		pc += MEM_SIZE / arena->carriages_nbr;
		players = players->next;
	}
	arena->live_id = -ft_reverse_bytes(carriage->regs[1]);
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

	carriage = carriages->content;
	if (carriage->death == 0 && (carriage->arena->nbr_cycles + 1 - carriage->last_live_cycle) >= carriage->arena->cycles_to_die)
		{
			carriage->death = 1;
			carriage->arena->carriages_nbr--;
		}
}

int ft_check_arena(t_arena *arena)
{
	if (arena->cycles_to_die > 0 && ((arena->nbr_cycles - arena->cycle_change_cycles_to_die + 1) % arena->cycles_to_die)) 
		return(0);
	//ft_printf("Cycle #%d check\n", arena->nbr_cycles);
	ft_lstiter(arena->carriages, ft_mark_death_carriages);	
	if (arena->carriages_nbr == 0)
		return(1);
	if (arena->live_nbr >= NBR_LIVE || arena->checks_nbr >= MAX_CHECKS)
	{
		arena->cycles_to_die -= CYCLE_DELTA;
		arena->checks_nbr = 0;
		arena->cycle_change_cycles_to_die = arena->nbr_cycles + 1;
	}
	else 
		arena->checks_nbr++;
	arena->live_nbr = 0;
	return (0);
}

void	ft_check_op(t_carriage *carriage)
{
	if (carriage->op > 0 && carriage->op < 17)
	{
		carriage->cycles_to_exec = op_tab[carriage->op].cycles_to_exec - 2;
		carriage->wait_cmd = 0;
	//-----debug
	//	ft_printf("found op - \"%s\"\n", op_tab[carriage->op].name);
	//-----debug
	}
	else
		carriage->pc = (carriage->pc + 1) % MEM_SIZE;
}

void ft_run_op(t_carriage *carriage)
{
	t_arg_byte	arg;
	int args[3];
	
	arg.byte = carriage->core[(carriage->pc + 1) % MEM_SIZE];
	args[0] = arg.value.at8;
	args[1] = arg.value.at6;
	args[2] = arg.value.at4;
	op_tab[carriage->op].f(carriage, args);
}

void ft_run_carriages(t_list *carriages)
{
	t_carriage *carriage;
	
	carriage = carriages->content;
	if (carriage->death)
		return;
	if (!carriage->cycles_to_exec)
	{
		if (carriage->wait_cmd)
		{
			carriage->op = carriage->core[carriage->pc];
			ft_check_op(carriage);
		}
		else
		{
			ft_run_op(carriage);
		}
		return;
	}
	carriage->cycles_to_exec--;
}
void ft_introduce(t_list *players)
{
	t_player *player;

	player = players->content;
	ft_printf("* Player %d, weighing %d bytes, \"%s\" (\"%s\") !\n",
	player->nbr, ft_reverse_bytes(player->header.prog_size),
	player->header.prog_name, player->header.comment);
}
void ft_start_game(t_arena *arena)
{
	ft_printf("Introducing contestants...\n");
	ft_lstiter(arena->players, ft_introduce);
	while(1)
	{
		if (arena->dump_nbr_cycles && *(arena->dump_nbr_cycles) == arena->nbr_cycles)
		{
			ft_print_memory(arena->core, MEM_SIZE);
			break;
		}
	 	ft_lstiter(arena->carriages, ft_run_carriages);
		if (ft_check_arena(arena))
			{
				if (ft_get_player(arena, arena->live_id))
					ft_printf("Contestant %d, \"%s\", has won !\n", arena->live_id, ft_get_player(arena, arena->live_id)->header.prog_name);
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
	//ft_print_memory(arena.core, MEM_SIZE);
	//ft_cleanup(&arena);
	return (0);
}
