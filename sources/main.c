/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/08 20:32:21 by majosue           #+#    #+#             */
/*   Updated: 2020/10/01 06:41:38 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

t_op    g_op_tab[17] =
{
	{0,				0, {0},														0,		0, 			0, 									0, 		0, 0},
	{"live",		1, {T_DIR},													1,		10, 		"alive", 							0, 		0, ft_live},
	{"ld",			2, {T_DIR | T_IND, T_REG},									2,		5, 			"load", 							1, 		0, ft_ld_lld},
	{"st",			2, {T_REG, T_IND | T_REG},									3,		5, 			"store", 							1, 		0, ft_st},
	{"add",			3, {T_REG, T_REG, T_REG},									4,		10, 		"addition", 						1, 		0, ft_add_sub},
	{"sub",			3, {T_REG, T_REG, T_REG},									5,		10, 		"soustraction",						1, 		0, ft_add_sub},
	{"and", 		3, {T_REG | T_DIR | T_IND, T_REG | T_IND | T_DIR, T_REG},	6,		6, 			"et (and r1, r2, r3 r1&r2 -> r3",	1, 		0, ft_and_or_xor},
	{"or",			3, {T_REG | T_IND | T_DIR, T_REG | T_IND | T_DIR, T_REG},	7,		6, 			"ou  (or r1, r2, r3 r1|r2 -> r3", 	1, 		0, ft_and_or_xor},
	{"xor",			3, {T_REG | T_IND | T_DIR, T_REG | T_IND | T_DIR, T_REG},	8,		6, 			"ou (xor r1, r2, r3 r1^r2 -> r3", 	1, 		0, ft_and_or_xor},
	{"zjmp",		1, {T_DIR},													9,		20, 		"jump if zero", 					0, 		1, ft_zjmp},
	{"ldi",			3, {T_REG | T_DIR | T_IND, T_DIR | T_REG, T_REG},			10,		25, 		"load index", 						1, 		1, ft_ldi_lldi},
	{"sti",			3, {T_REG, T_REG | T_DIR | T_IND, T_DIR | T_REG},			11,		25, 		"store index", 						1, 		1, ft_sti},
	{"fork",		1, {T_DIR},													12,		800, 		"fork",	 							0, 		1, ft_fork_lfork},
	{"lld",			2, {T_DIR | T_IND, T_REG},									13,		10,			"long load", 						1, 		0, ft_ld_lld},
	{"lldi",		3, {T_REG | T_DIR | T_IND, T_DIR | T_REG, T_REG},			14,		50,			"long load index", 					1, 		1, ft_ldi_lldi},
	{"lfork",		1, {T_DIR},													15,		1000,		"long fork", 						0, 		1, ft_fork_lfork},
	{"aff",			1, {T_REG},													16,		2, 			"aff", 								1, 		0, ft_aff}
};

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
	while (i < g_op_tab[carriage->op].max_params)
	{
		skip += g_params[g_op_tab[carriage->op].short_dir][args[i]];
		i++;
	}
	carriage->pc = (carriage->pc + skip + addon) % MEM_SIZE;
	carriage->wait_cmd = 1;
}

/*
**	Check valid registry number 
**	store adress of registry to carriage->params
**	store number to carriage->reg_nbrs
*/

int	ft_is_valid_regs(t_carriage *carriage, int args[3])
{
	int i;
	int pc;

	i = 0;
	pc = (carriage->pc + 2) % MEM_SIZE;
	while (i < g_op_tab[carriage->op].max_params)
	{
		if (args[i] == REG_CODE)
		{
			if (carriage->core[pc] <= 0 || carriage->core[pc] > REG_NUMBER)
				return (0);
			carriage->params[i] = &(carriage->regs[carriage->core[pc]]);
			carriage->reg_nbrs[i] = carriage->core[pc];
		}
			pc = (pc + g_params[g_op_tab[carriage->op].short_dir][args[i]])
			% MEM_SIZE;
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

/*
**	Store adress of args to carriage->params
**	store IND values to carriage->ind_val
*/

void ft_load_params(t_carriage *carriage, int args[3], int mod, int arg_bytes)
{
	int pc;
	int ind_pc;
	int i;
	short ind;
	
	pc = (carriage->pc + 1 + arg_bytes) % MEM_SIZE;
	i = -1;
	while (++i < g_op_tab[carriage->op].max_params)
	{
		if (args[i] == DIR_CODE)
			carriage->params[i] = &carriage->core[pc];
		if (args[i] == IND_CODE)
		{
			ft_memmove_circle(&ind, &carriage->core[pc],
			carriage, sizeof(short));
			ind = ft_reverse_bytes_short(ind);
			carriage->ind_val[i] = ind;
			ind = mod ? ind % IDX_MOD : ind;
			ind_pc = ft_calculate_pc(carriage, ind);
			carriage->params[i] = &carriage->core[ind_pc];
		}
		pc = (pc + g_params[g_op_tab[carriage->op].short_dir][args[i]])
		% MEM_SIZE;
	}
}

/*
**	Store values at carriage->params in carriage->values
**	use DIR_SIZE instead others (we alrady get adreses in load_params)
**	use "g_op_tab[carriage->op].op_code == 0x0d ? 2 :size;"
**	for repeat behavour of original corewar
**	op 0x0d lld read 2 bytes on IND adress
*/

void ft_load_values(t_carriage *carriage, int args[3])
{
	int i;
	int size;
	short buffer;
	
	i = 0;
	while (i < g_op_tab[carriage->op].max_params)
	{
		size = args[i] == IND_CODE || args[i] == REG_CODE ? DIR_SIZE :
		g_params[g_op_tab[carriage->op].short_dir][args[i]];
		size = args[i] == IND_CODE &&
		g_op_tab[carriage->op].op_code == 0x0d ? 2 :size;
	if (size < DIR_SIZE)
	{
		ft_memmove_circle(&buffer, carriage->params[i], carriage, size);
		buffer = ft_reverse_bytes_short(buffer);
		carriage->values[i] = buffer;
	}
	else 
		{
		ft_memmove_circle(&carriage->values[i], carriage->params[i], carriage,
		size);
		carriage->values[i] = ft_reverse_bytes(carriage->values[i]);
		}
	i++;
	}
}

int ft_live(t_carriage *carriage, int args[3])
{
	t_player *player;
	int id;
	
	args[0] = DIR_CODE;
	ft_load_params(carriage, args, 1, 0);
	ft_load_values(carriage, args);
	id = -carriage->values[0];
	ft_skip_args(carriage, args, 1);
	if ((player = ft_get_player(carriage->arena, id)))
	{
		if (carriage->arena->verbose & 1)
			ft_printf("A process shows that player %d (%s) is alive\n",
		id, player->header.prog_name);
		carriage->arena->live_id = id;
	}
	carriage->arena->live_nbr++;
	carriage->last_live_cycle = carriage->arena->nbr_cycles;
	return (EXIT_SUCCESS);
}

int ft_ld_lld(t_carriage *carriage, int args[3])
{
	int mod;
	int result;

	if (!((args[0] == DIR_CODE || args[0] == IND_CODE) &&
	args[1] == REG_CODE && ft_is_valid_regs(carriage, args)))
	{
		ft_skip_args(carriage, args, 2);
		return (EXIT_FAILURE);
	}
	mod = g_op_tab[carriage->op].op_code == 2 ? 1 : 0;
	ft_load_params(carriage, args, mod, 1);
	ft_load_values(carriage, args);
	result = ft_reverse_bytes(carriage->values[0]);
	ft_memmove_circle(carriage->params[1],
	&result, carriage, REG_SIZE);
	ft_load_values(carriage, args);
	carriage->carry = carriage->values[1] ? 0 : 1;
	ft_skip_args(carriage, args, 2);
	return (EXIT_SUCCESS);
}

int ft_st(t_carriage *carriage, int args[3])
{
	if (!(args[0] == REG_CODE  && (args[1] == REG_CODE ||
	args[1] == IND_CODE) && ft_is_valid_regs(carriage, args)))
	{
		ft_skip_args(carriage, args, 2);
		return (EXIT_FAILURE);
	}
	ft_load_params(carriage, args, 1, 1);
	ft_memmove_circle(carriage->params[1],
	carriage->params[0], carriage, REG_SIZE);
	ft_load_values(carriage, args);
	if ((unsigned char*)carriage->params[1] - carriage->core >= 0 &&
	(unsigned char*)carriage->params[1] - carriage->core < MEM_SIZE)
	carriage->values[1] = carriage->ind_val[1];
	ft_skip_args(carriage, args, 2); 
	return (EXIT_SUCCESS);
}

int ft_add_sub (t_carriage *carriage, int args[3])
{
	int result;

	if (!(args[0] == REG_CODE && args[1] == REG_CODE &&
args[2] == REG_CODE && ft_is_valid_regs(carriage, args)))
	{
		ft_skip_args(carriage, args, 2);
		return (EXIT_FAILURE);
	}
	ft_load_values(carriage, args);
	if (g_op_tab[carriage->op].op_code == 4)
		carriage->values[2] = carriage->values[0] + carriage->values[1];
	else 
		carriage->values[2] = carriage->values[0] - carriage->values[1];
	result = ft_reverse_bytes(carriage->values[2]);
	ft_memmove(carriage->params[2], &result, REG_SIZE);
	ft_skip_args(carriage, args, 2);
	carriage->carry = carriage->values[2] ? 0 : 1;
	return (EXIT_SUCCESS);
}

int ft_and_or_xor(t_carriage *carriage, int args[3])
{
	int result;

	if (!(args[0] != 0 && args[1] != 0 && args[2] == REG_CODE &&
ft_is_valid_regs(carriage, args)))
	{
		ft_skip_args(carriage, args, 2);
		return (EXIT_FAILURE);
	}
	ft_load_params(carriage, args, 1, 1);
	ft_load_values(carriage, args);
	if (g_op_tab[carriage->op].op_code == 6)
		carriage->values[2] = carriage->values[0] & carriage->values[1];
	else if (g_op_tab[carriage->op].op_code == 7)
		carriage->values[2] = carriage->values[0] | carriage->values[1];
	else 
		carriage->values[2] = carriage->values[0] ^ carriage->values[1];
	result = ft_reverse_bytes(carriage->values[2]);	
	ft_memmove_circle(carriage->params[2], &result, carriage, REG_SIZE);
	ft_skip_args(carriage, args, 2);
	carriage->carry = carriage->values[2] ? 0 : 1;
	return (EXIT_SUCCESS);
}

int ft_zjmp(t_carriage *carriage, int args[3])
{
	int pc;

	args[0] = DIR_CODE;
	ft_load_params(carriage, args, 1, 0);
	ft_load_values(carriage, args);
	pc = carriage->values[0];
	pc = pc % IDX_MOD;
	pc = ft_calculate_pc(carriage, pc);
	if (carriage->carry == 0)
	{
			ft_skip_args(carriage, args, 1);
			return(EXIT_FAILURE);
	}
	carriage->pc = pc;
	carriage->wait_cmd = 1;
	return(EXIT_SUCCESS);
}

int ft_ldi_lldi(t_carriage *carriage, int args[3])
{
	int pc;

	if (!(args[0] != 0 && (args[1] == DIR_CODE ||
args[1] == REG_CODE) && args[2] == REG_CODE &&
ft_is_valid_regs(carriage, args)))
	{
		ft_skip_args(carriage, args, 2);
		return (EXIT_FAILURE);
	}
	ft_load_params(carriage, args, 1, 1);
	ft_load_values(carriage, args);
	carriage->values[2] = carriage->values[0] + carriage->values[1];
	carriage->binary_op_result = carriage->values[2];
	carriage->values[2] = g_op_tab[carriage->op].op_code == 10 ?
	carriage->values[2] % IDX_MOD : carriage->values[2];
	carriage->target_pc = carriage->values[2] + carriage->pc;
	pc = ft_calculate_pc(carriage, carriage->values[2]);
	ft_memmove_circle(carriage->params[2], &carriage->core[pc],
	carriage, REG_SIZE);
	if (g_op_tab[carriage->op].op_code == 14)
		carriage->carry = *(int *)carriage->params[2] ? 0 : 1;
	ft_skip_args(carriage, args, 2);
	return (EXIT_SUCCESS);
}

int ft_sti(t_carriage *carriage, int args[3])
{
	int pc;

	if (!(args[0] == REG_CODE && args[1] != 0 &&
(args[2] == DIR_CODE || args[2] == REG_CODE)
&& ft_is_valid_regs(carriage, args)))
	{
		ft_skip_args(carriage, args, 2);
		return (EXIT_FAILURE);
	}
	ft_load_params(carriage, args, 1, 1);
	ft_load_values(carriage, args);
	carriage->values[0] = carriage->values[1] + carriage->values[2];
	carriage->binary_op_result = carriage->values[0];
	carriage->values[0] = carriage->values[0] % IDX_MOD;
	carriage->target_pc = carriage->values[0] + carriage->pc;
	pc = ft_calculate_pc(carriage, carriage->values[0]);
	ft_memmove_circle(&carriage->core[pc], carriage->params[0], carriage, REG_SIZE);
	ft_skip_args(carriage, args, 2);
	return (EXIT_SUCCESS);
}

int ft_fork_lfork(t_carriage *carriage, int args[3])
{
	int mod;
	t_carriage 	*new_carriage;
	t_list		*new_node;

	mod = g_op_tab[carriage->op].op_code == 12 ? 1 : 0;
	if (!(new_carriage = (t_carriage*)ft_memalloc(sizeof(t_carriage))))
		ft_exit("ERROR", NULL);
	ft_memmove(new_carriage, carriage, sizeof(t_carriage));
	args[0] = DIR_CODE;
	ft_load_params(carriage, args, mod, 0);
	ft_load_values(carriage, args);
	new_carriage->pc = carriage->values[0];
	new_carriage->pc = g_op_tab[carriage->op].op_code == 12 ? 
	new_carriage->pc % IDX_MOD : new_carriage->pc;
	carriage->target_pc = new_carriage->pc + carriage->pc;
	new_carriage->pc = ft_calculate_pc(carriage, new_carriage->pc);
	new_carriage->id = ++carriage->arena->carriage_index;
	if (!(new_node = ft_lstnew(new_carriage, sizeof(*new_carriage))))
		ft_exit("ERROR", NULL);
	ft_lstadd(&carriage->arena->carriages, new_node);
	carriage->arena->carriages_nbr++;
	new_carriage->wait_cmd = 1;
	ft_skip_args(carriage, args, 1);
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
	c = carriage->values[0];
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
	arena->carriage_index = 0;
	arena->carriages = NULL;
	arena->dump_nbr_cycles = NULL;
	arena->players = NULL;
	arena->cycles_to_die = CYCLE_TO_DIE;
	arena->cycle_change_cycles_to_die = 0;
	arena->live_nbr = 0;
	arena->nbr_cycles = 0;
	arena->checks_nbr = 0;
	arena->verbose = 1;
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
		else if (ft_strequ(argv[i], "-v"))
		{
			ft_check_next_args(i, argc, argv, 1);
			arena->verbose = ft_atoi(argv[++i]);
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
		carriage->id = ((t_player*)players->content)->nbr;
		carriage->regs[1] = ft_reverse_bytes(-((t_player*)players->content)->nbr); 
		if (!(new_carriage = (ft_lstnew(carriage, sizeof(*carriage)))))
			ft_exit("ERROR", NULL);
		ft_lstadd(&arena->carriages, new_carriage);
		ft_memmove(arena->core + pc, ((t_player*)players->content)->code,
		ft_reverse_bytes(((t_player*)players->content)->header.prog_size));
		pc += MEM_SIZE / arena->carriages_nbr;
		players = players->next;
	}
	arena->live_id = -ft_reverse_bytes(carriage->regs[1]);
	carriage->arena->carriage_index = carriage->arena->carriages_nbr;
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
	if (carriage->death == 0 && (carriage->arena->nbr_cycles - carriage->last_live_cycle) >= carriage->arena->cycles_to_die)
		{
			carriage->death = 1;
			carriage->arena->carriages_nbr--;
			if (carriage->arena->verbose & 8)
				ft_printf("Process %d hasn't lived for %d cycles (CTD %d)\n", 
				carriage->id, carriage->arena->nbr_cycles - carriage->last_live_cycle,
				carriage->arena->cycles_to_die);
		}
}

int ft_check_arena(t_arena *arena)
{
	if (arena->cycles_to_die > 0 && ((arena->nbr_cycles - arena->cycle_change_cycles_to_die) &&
	(arena->nbr_cycles - arena->cycle_change_cycles_to_die) % arena->cycles_to_die)) 
		return(0);
	ft_lstiter(arena->carriages, ft_mark_death_carriages);	
	if (arena->carriages_nbr == 0)
		return(1);
	if (arena->live_nbr >= NBR_LIVE || arena->checks_nbr >= MAX_CHECKS)
	{
		arena->cycles_to_die -= CYCLE_DELTA;
		arena->checks_nbr = 0;
		arena->cycle_change_cycles_to_die = arena->nbr_cycles;
	}
	arena->checks_nbr++;
	arena->live_nbr = 0;
	return (0);
}

void ft_print_sti(t_carriage *carriage)
{
	ft_printf("r%d %d %d\n", carriage->reg_nbrs[0], carriage->values[1], carriage->values[2]);
	ft_printf("       | -> store to %d + %d = %d (with pc and mod %d)\n", carriage->values[1], carriage->values[2], carriage->binary_op_result, carriage->target_pc);
}

void ft_print_ldi(t_carriage *carriage)
{
	ft_printf("%d %d r%d\n", carriage->values[0], carriage->values[1], carriage->reg_nbrs[2]);
	ft_printf("       | -> load from %d + %d = %d (with pc and mod %d)\n", carriage->values[0], carriage->values[1], carriage->binary_op_result, carriage->target_pc);
}

void ft_print_zjump(t_carriage *carriage)
{
	if (carriage->carry == 0)
		ft_printf("%d FAILED\n", carriage->values[0]);
	else
		ft_printf("%d OK\n", carriage->values[0]);
}

void ft_print_common_op(t_carriage *carriage, int args[3])
{
	int i;
	
	i = -1;
	while (++i < g_op_tab[carriage->op].max_params)
		{
			if (args[i] == REG_CODE)
				ft_printf("r%d ", carriage->reg_nbrs[i]);
			else
				ft_printf("%d ", carriage->values[i]);	
		}
	ft_printf("\n");
}

void ft_print_op(t_carriage *carriage, int result, int args[3])
{
	if (result && g_op_tab[carriage->op].op_code != 9)
		return;
	ft_printf("P%5d | %s ", carriage->id, g_op_tab[carriage->op].name);
	if (g_op_tab[carriage->op].op_code == 12 || g_op_tab[carriage->op].op_code == 15)
		ft_printf("%d (%d)\n", carriage->values[0], carriage->target_pc);
	else if (g_op_tab[carriage->op].op_code == 11)
		ft_print_sti(carriage);
	else if (g_op_tab[carriage->op].op_code == 10 
	|| g_op_tab[carriage->op].op_code == 14)
		ft_print_ldi(carriage);
	else if (g_op_tab[carriage->op].op_code == 6 ||
	g_op_tab[carriage->op].op_code == 7 ||
	g_op_tab[carriage->op].op_code == 8)
		ft_printf("%d %d r%d\n", carriage->values[0], carriage->values[1],
		carriage->reg_nbrs[2]);
	else if (g_op_tab[carriage->op].op_code == 9)
		ft_print_zjump(carriage);
	else
		ft_print_common_op(carriage, args); 
}

void	ft_check_op(t_carriage *carriage)
{
	if (carriage->op > 0 && carriage->op < 17)
	{
		carriage->cycles_to_exec = g_op_tab[carriage->op].cycles_to_exec - 2;
		carriage->wait_cmd = 0;
	}
	else
		carriage->pc = (carriage->pc + 1) % MEM_SIZE;
}

void ft_run_op(t_carriage *carriage)
{
	t_arg_byte	arg;
	int args[3];
	int result;
	
	arg.byte = carriage->core[(carriage->pc + 1) % MEM_SIZE];
	args[0] = arg.value.at8;
	args[1] = arg.value.at6;
	args[2] = arg.value.at4;
	result = g_op_tab[carriage->op].f(carriage, args);
	if (carriage->arena->verbose & 4)
		ft_print_op(carriage, result, args);
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
		(arena->nbr_cycles)++;
	 	if (arena->verbose & 2)
			ft_printf("It is now cycle %d\n", arena->nbr_cycles);
		 ft_lstiter(arena->carriages, ft_run_carriages);
		if (ft_check_arena(arena))
			{
				if (ft_get_player(arena, arena->live_id))
					ft_printf("Contestant %d, \"%s\", has won !\n", arena->live_id, ft_get_player(arena, arena->live_id)->header.prog_name);
				break;
			}
	}	
}

void del(void *memory, size_t size)
{
	ft_bzero(memory, size);
	ft_memdel(&memory);
}

void del_player(void *memory, size_t size)
{
	ft_bzero(((t_player *)memory)->code, sizeof(t_player));
	ft_memdel(&((t_player *)memory)->code);
	ft_bzero(memory, size);
	ft_memdel(&memory);
}

void ft_cleanup(t_arena *arena)
{
	ft_lstdel(&arena->carriages, del);
	ft_lstdel(&arena->players, del_player);
	free(arena->dump_nbr_cycles);
}

int main(int argc, char **argv)
{
	t_arena arena;

	if (argc < 2)
		ft_exit("\
usage: ./corewar [-dump N -v N] [[-n N] champion1.cor] ...\n\
	-dump N	: Dumps memory after N cycles then exits\n\
	-v N : Verbosity levels, can be added together to enable several\n\
		- 0 : Show only essentials\n\
		- 1 : Show lives (default)\n\
		- 2 : Show cycles\n\
		- 4 : Show operations (Params are NOT litteral ...)\n\
		- 8 : Show deaths\n\
	-n N : Set unic champion number", "");
	ft_init_arena(&arena);
	ft_read_args(&arena, argc, argv);
	ft_put_players_to_arena(&arena);
	ft_start_game(&arena);
	ft_cleanup(&arena);
	return (0);
}
