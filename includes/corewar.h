//
// Created by Haako Trent on 9/7/20.
//

#ifndef COREWAR_COREWAR_H
#define COREWAR_COREWAR_H

# include "libft.h"
# include "ft_printf.h"
# include "op.h"

# define DUMP_WIDTH 64 // subject ask us chenge it to 32 

typedef struct s_player
{
    int         nbr;
    t_header    header;
    void        *code;
}               t_player;

typedef struct  s_arena
{
    int             *dump_nbr_cycles;
    int             carriages_nbr;
    int             carriage_index;
    unsigned char   core[MEM_SIZE];
    t_list          *players;        
    t_list          *carriages;
    int             nbr_cycles;
    int             cycles_to_die;
    id_t            cycle_change_cycles_to_die;
    int             live_nbr;
    int             live_id;
    int             checks_nbr;
    int         dbg_counter;
}               t_arena;

typedef struct s_carriage
{
    int         id;
    int         pc;
    int         wait_cmd;
   // int         wait_args;
    unsigned char   *core;
    int         regs[REG_NUMBER + 1];
    int         carry;
    unsigned char         op;
    int         cycles_to_exec;
    int         last_live_cycle;
    int         next_op_distance;
    int         death;
    void        *params[3];
    int         values[3];
    int         reg_nbrs[3];
    int         ind_val[3];
    int         tmp1;
    int         tmp2;
    int         target_pc;
   /*  int         *cycles_to_die;
    int         *nbr_cycles;
    int         *carriages_nbr; */
    t_arena     *arena;
}               t_carriage;



struct s_byte
{
    unsigned at2: 2;
    unsigned at4: 2;
    unsigned at6: 2;
    unsigned at8: 2;
};

typedef union u_byte
{
    unsigned char byte;
    struct s_byte value;
}               t_arg_byte;

typedef int    (*t_op_exec)(t_carriage *, int[3]);

typedef struct  s_op
{
    char    *name;
    int     max_params;
    t_arg_type  args[4];
    int    op_code;
    int    cycles_to_exec;
    char    *description;
    int     mod_carry;
    int     short_dir;
    t_op_exec f;
}               t_op;


unsigned int ft_reverse_bytes(unsigned int value);
unsigned short ft_reverse_bytes_short(unsigned short value);
t_player	*ft_get_player(t_arena *arena, int id);

void ft_exit(char *str, char *str2);
void ft_load_params(t_carriage *carriage, int args[3], int mod, int arg_bytes);
void ft_load_values(t_carriage *carriage, int args[3]);
int	ft_live_arg_check(t_carriage *carriage, int args[3]);
int	ft_ld_arg_check(t_carriage *carriage, int args[3]);
int	ft_st_arg_check(t_carriage *carriage, int args[3]);
int	ft_add_arg_check(t_carriage *carriage, int args[3]);
int	ft_and_arg_check(t_carriage *carriage, int args[3]);
int	ft_ldi_arg_check(t_carriage *carriage, int args[3]);
int	ft_sti_arg_check(t_carriage *carriage, int args[3]);
int	ft_sti_aff_check(t_carriage *carriage, int args[3]);
int ft_live(t_carriage *carriage, int args[3]);
int ft_ld_lld(t_carriage *carriage, int args[3]);
int ft_st(t_carriage *carriage, int args[3]);
int ft_add_sub (t_carriage *carriage, int args[3]);
int ft_and_or_xor (t_carriage *carriage, int args[3]);
int ft_zjmp(t_carriage *carriage, int args[3]);
int ft_ldi_lldi(t_carriage *carriage, int args[3]);
int ft_sti(t_carriage *carriage, int args[3]);
int ft_fork_lfork(t_carriage *carriage, int args[3]);
int ft_aff(t_carriage *carriage, int args[3]);

void ft_print_memory(void *mem, size_t size);

#endif //COREWAR_COREWAR_H
