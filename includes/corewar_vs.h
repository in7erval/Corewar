//
// Created by Дмитрий Юдаков on 03.10.2020.
//

#ifndef COREWAR_VS_H
#define COREWAR_VS_H

# include "libft.h"
# include "ft_printf.h"
# include "op.h"
# include <stdint.h>
# include <stdbool.h>
# include <ncurses.h>
# include "corewar.h"


# define WIN_WIDTH	250
# define CORE_WIDTH 196
# define WIN_HEIGHT 67
# define ESC	27

# define DUMP_WIDTH 64

# define COLOR_GRAY 	8
# define GRAY			9
# define GREEN			10
# define YELLOW			11
# define RED			12
# define CYAN			13
# define GRAY_CARR		14
# define GREEN_CARR		15
# define YELLOW_CARR	16
# define RED_CARR		17
# define CYAN_CARR		18
# define GREEN_LIVE		20
# define YELLOW_LIVE	21
# define RED_LIVE		22
# define CYAN_LIVE		23
# define COMMON			24
# define BORDER			25

static int g_colors[] = {
		COLOR_PAIR(GRAY),
		COLOR_PAIR(GREEN),
		COLOR_PAIR(YELLOW),
		COLOR_PAIR(RED),
		COLOR_PAIR(CYAN),
		COLOR_PAIR(GRAY_CARR), //5
		COLOR_PAIR(GREEN_CARR),
		COLOR_PAIR(YELLOW_CARR),
		COLOR_PAIR(RED_CARR),
		COLOR_PAIR(CYAN_CARR),
		COLOR_PAIR(COMMON),
		COLOR_PAIR(GREEN_LIVE), //11
		COLOR_PAIR(YELLOW_LIVE),
		COLOR_PAIR(RED_LIVE),
		COLOR_PAIR(CYAN_LIVE)
};

typedef struct s_player t_player;
typedef struct s_carriage t_carriage;
typedef struct s_arena t_arena;

typedef struct 			s_attr
{
	size_t 				value;
	size_t 				cycles_live;
	size_t 				cycles_store;
	t_player			*player;
}						t_attr;

typedef struct 			s_visual
{
	t_attr				map[MEM_SIZE]; //map with attributes
	int 				button;
	clock_t 			time;
	int 				cycles_per_sec;
	int 				debug;
	int 				is_running;
}						t_visual;

void 					place_carriage(t_arena *arena, t_carriage *carriage);
void 					remove_carriage(t_arena *arena, t_carriage *carriage);
void					visualize(t_arena *arena);
void 					update_map(t_arena *arena, t_carriage *carriage, int addr, int size);

#endif
