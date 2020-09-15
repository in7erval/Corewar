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
    int         regs[REG_NUMBER];
    int         carry;
    int         op;
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


#endif //COREWAR_COREWAR_H
