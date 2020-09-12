//
// Created by Haako Trent on 9/7/20.
//

#ifndef COREWAR_COREWAR_H
#define COREWAR_COREWAR_H

# include "libft.h"
# include "ft_printf.h"
# include "op.h"
# include <stdint.h>
# include <stdbool.h>
# include <ncurses.h>


# define WIN_WIDTH	250
# define CORE_WIDTH 196
# define WIN_HEIGHT 67

# define DUMP_WIDTH 64 // subject ask us chenge it to 32


typedef struct			s_player
{
	char 				*name;
	char 				*comment;
	int         		player_nbr;
	void        		*player_code;
	int 				prog_size;
	t_header    		header;

}						t_player;

typedef struct		s_carriage
{
    int         	pc;
    unsigned char	*pos;
    int         	regs[REG_NUMBER];
    int         	carry;
	t_player 		*owner;
}               	t_carriage;

typedef struct		s_arena
{
    int             *dump_nbr_cycles;
    int             carriages_nbr;
    unsigned char   core[MEM_SIZE];        
    t_list          *carriages;
    t_list 			*players;
    size_t 			players_nbr;
}               	t_arena;

typedef struct 		s_visual
{
	size_t 			map[MEM_SIZE]; //map with attributes
}					t_visual;

void		visualize(t_arena *arena);
unsigned	ft_reverse_bytes(unsigned int value);
void 		ft_exit(char *str, char *str2);

#endif //COREWAR_COREWAR_H
