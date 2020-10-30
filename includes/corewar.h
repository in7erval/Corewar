/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   corewar.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 05:08:50 by majosue           #+#    #+#             */
/*   Updated: 2020/10/27 10:15:38 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COREWAR_COREWAR_H
# define COREWAR_COREWAR_H

# include "libft.h"
# include "ft_printf.h"
# include "op.h"
# include "corewar_vs.h"

typedef struct s_visual	t_visual;

typedef struct			s_player
{
	int					nbr;
	t_header			header;
	void				*code;
	int					current_lives;
	int					last_live;
	int					nbr_color;
}						t_player;

typedef struct			s_arena
{
	int					dump_nbr_cycles;
	int					carriages_nbr;
	int					carriage_index;
	unsigned char		core[MEM_SIZE];
	t_list				*players;
	t_list				*carriages;
	int					nbr_cycles;
	int					cycles_to_die;
	int					cycle_change_cycles_to_die;
	int					live_nbr;
	int					live_id;
	int					checks_nbr;
	unsigned char		verbose;
	int					legacy;
	t_visual			*visual;
	int					v;
	int					a;
	int					after_check;
}						t_arena;

typedef struct			s_carriage
{
	int					id;
	t_player			*owner;
	int					pc;
	int					wait_cmd;
	unsigned char		*core;
	int					regs[REG_NUMBER + 1];
	int					carry;
	unsigned char		op;
	int					cycles_to_exec;
	int					last_live_cycle;
	int					death;
	void				*params[3];
	int					values[3];
	int					reg_nbrs[3];
	int					ind_val[3];
	int					binary_op_result;
	int					target_pc;
	t_arena				*arena;
}						t_carriage;

struct					s_byte
{
	unsigned at2: 2;
	unsigned at4: 2;
	unsigned at6: 2;
	unsigned at8: 2;
};

typedef union			u_byte
{
	unsigned char	byte;
	struct s_byte	value;
}						t_arg_byte;

typedef int	(*t_op_exec)(t_carriage *, int[3]);

typedef struct			s_op
{
	char		*name;
	int			max_params;
	t_arg_type	args[4];
	int			op_code;
	int			cycles_to_exec;
	char		*description;
	int			mod_carry;
	int			short_dir;
	t_op_exec	f;
}						t_op;

extern t_op				g_op_tab[17];
extern unsigned char	g_params[2][4];
unsigned int			ft_reverse_bytes(unsigned int value);
unsigned short			ft_reverse_bytes_short(unsigned short value);

void					ft_start_game(t_arena *arena);

t_player				*ft_get_player(t_arena *arena, int id);
void					ft_exit(char *str, char *str2);
void					ft_load_params(t_carriage *carriage, const int args[3],
	int mod, int arg_bytes);
void					ft_load_values(t_carriage *carriage, int args[3]);
int						ft_live(t_carriage *carriage, int args[3]);
int						ft_ld_lld(t_carriage *carriage, int args[3]);
int						ft_st(t_carriage *carriage, int args[3]);
int						ft_add_sub (t_carriage *carriage, int args[3]);
int						ft_and_or_xor (t_carriage *carriage, int args[3]);
int						ft_zjmp(t_carriage *carriage, int args[3]);
int						ft_ldi_lldi(t_carriage *carriage, int args[3]);
int						ft_sti(t_carriage *carriage, int args[3]);
int						ft_fork_lfork(t_carriage *carriage, int args[3]);
int						ft_aff(t_carriage *carriage, int args[3]);
void					ft_print_memory(void *mem, size_t size);
void					ft_memmove_circle(void *dst, void *src,
	t_carriage *carriage, size_t size);
int						ft_is_valid_regs(t_carriage *carriage, int args[3]);
void					ft_skip_args(t_carriage *carriage,
	int args[3], int addon);
int						ft_calculate_pc(t_carriage *carriage, int addon);
void					ft_read_args(t_arena *arena, int argc, char **argv);
int						ft_find_next_avialable_number(t_list *players,
	int delta);
void					ft_check_next_args(int current, int number,
	char **argv, int delta);
void					ft_set_dump(char *str, t_arena *arena);
void					ft_cleanup(t_arena *arena);
void					ft_read_champion(char **str, char **file,
	t_arena *arena);
void					ft_print_op(t_carriage *carriage,
	int result, int args[3]);
int						ft_check_arena(t_arena *arena);

void					ft_run_carriages(t_list *carriages);
int						print_usage(void);
void					ft_init_arena(t_arena *arena);
void					ft_put_players_to_arena(t_arena *arena);
t_carriage				*ft_create_carriage(t_arena *arena, t_player *player);
t_carriage				*duplicate_carriage(t_carriage *carriage);

#endif
