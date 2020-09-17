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

typedef struct s_carriage
{
    int         pc;
    int         wait_cmd;
    int         wait_args;
    unsigned char   *core;
    int         regs[REG_NUMBER];
    int         carry;
    unsigned char         op;
    int         cycles_to_exec;
    int         last_live_cycle;
    int         next_op_distance;
    int         death;
    int         *cycles_to_die;
    int         *nbr_cycles;
    int         *carriages_nbr;
}               t_carriage;

typedef struct  s_arena
{
    int             *dump_nbr_cycles;
    int             carriages_nbr;
    unsigned char   core[MEM_SIZE];
    t_list          *players;        
    t_list          *carriages;
    int             nbr_cycles;
    int             cycles_to_die;
    int             live_nbr;
    int             live_id;
    int             checks_nbr;
}               t_arena;

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


typedef struct  s_op
{
    char    *name;
    int     max_params;
    t_arg_type  args[4];
    int    op_code;
    int    cycles_to_exec;
    char    *description;
    int     flag1;
    int     flag2;
}               t_op;

typedef int    (*t_arg_check)(t_carriage *, t_arg_byte);

int	ft_live_arg_check(t_carriage *carriage, t_arg_byte byte);
int	ft_ld_arg_check(t_carriage *carriage, t_arg_byte arg);
int	ft_st_arg_check(t_carriage *carriage, t_arg_byte arg);
int	ft_add_arg_check(t_carriage *carriage, t_arg_byte arg);
int	ft_and_arg_check(t_carriage *carriage, t_arg_byte arg);
int	ft_ldi_arg_check(t_carriage *carriage, t_arg_byte arg);
int	ft_sti_arg_check(t_carriage *carriage, t_arg_byte arg);
int	ft_sti_aff_check(t_carriage *carriage, t_arg_byte arg);

#endif //COREWAR_COREWAR_H
