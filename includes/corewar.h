//
// Created by Haako Trent on 9/7/20.
//

#ifndef COREWAR_COREWAR_H
#define COREWAR_COREWAR_H

# include "libft.h"
# include "ft_printf.h"
# include "op.h"

# define DUMP_WIDTH 64 // subject ask us chenge it to 32 

typedef struct s_carriage
{
    int         pc;
    unsigned char *pos;
    int         regs[REG_NUMBER];
    int         carry;
    int         player_nbr;
    void        *player_code;
    t_header    header;
}               t_carriage;

typedef struct  s_arena
{
    int             *dump_nbr_cycles;
    int             carriages_nbr;
    unsigned char   core[MEM_SIZE];        
    t_list          *carriages;
}               t_arena;

#endif //COREWAR_COREWAR_H
