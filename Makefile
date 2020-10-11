.PHONY: all clean fclean re

COREWAR = corewar
ASM = asm
CC = gcc
FLAGS = -Wall -Werror -Wextra -O3 -g
LIBRARIES = -lft -L$(LIBFT_DIRECTORY) -lncurses

INCLUDES = -I$(HEADERS_DIRECTORY) -I$(LIBFT_HEADERS)

LIBFT = $(LIBFT_DIRECTORY)libft.a
LIBFT_DIRECTORY = ./libft/
LIBFT_HEADERS = $(LIBFT_DIRECTORY)includes/

HEADERS_LIST = corewar.h corewar_vs.h
HEADERS_DIRECTORY = ./includes/
HEADERS = $(addprefix $(HEADERS_DIRECTORY), $(HEADERS_LIST))



SOURCES_DIRECTORY = ./sources/

COREWAR_SOURCES_DIRECTORY = $(SOURCES_DIRECTORY)corewar/
COREWAR_SOURCES_LIST = main.c \
					ft_start_game.c \
					ft_check_arena.c \
					ft_print_op.c \
					ft_memmove_circle.c \
					op_1_8.c \
					op_9_16.c \
					op_tools.c \
					ft_read_args.c \
					parser_tools.c \
					free_tools.c

VISUAL_SOURCES_DIRECTORY = $(SOURCES_DIRECTORY)visual/
VISUAL_SOURCES_LIST = colors.c \
					draw_borders.c \
					draw_core.c \
					draw_info.c \
					handle.c \
					init.c \
					map.c \
					util.c \
					visualize.c

VISUAL_SOURCES = $(addprefix $(VISUAL_SOURCES_DIRECTORY), $(VISUAL_SOURCES_LIST))
COREWAR_SOURCES = $(addprefix $(COREWAR_SOURCES_DIRECTORY), $(COREWAR_SOURCES_LIST))


OBJECTS_DIRECTORY = ./objects/

COREWAR_OBJECTS_DIRECTORY = $(OBJECTS_DIRECTORY)corewar/
COREWAR_OBJECTS_LIST = $(patsubst %.c, %.o, $(COREWAR_SOURCES_LIST))
COREWAR_OBJECTS	= $(addprefix $(COREWAR_OBJECTS_DIRECTORY), $(COREWAR_OBJECTS_LIST))

VISUAL_OBJECTS_DIRECTORY = $(OBJECTS_DIRECTORY)visual/
VISUAL_OBJECTS_LIST = $(patsubst %.c, %.o, $(VISUAL_SOURCES_LIST))
VISUAL_OBJECTS = $(addprefix $(VISUAL_OBJECTS_DIRECTORY), $(VISUAL_OBJECTS_LIST))

# COLORS
GREEN = \033[0;32m
BOLD_GREEN = \033[1;32m
RED = \033[0;31m
BOLD_RED = \033[1;31m
YELLOW = \033[0;33m
BOLD_YELLOW = \033[1;33m
BLUE = \033[0;34m
BOLD_BLUE = \033[1;34m
MAGENTA = \033[0;35m
BOLD_MAGENTA = \033[1;35m
CYAN = \033[0;36m
BOLD_CYAN = \033[1;36m
RESET = \033[0m


all: $(COREWAR)
	@echo "$(BOLD_RED) Ready! $(RESET)"

$(COREWAR): $(LIBFT) $(COREWAR_OBJECTS_DIRECTORY) $(VISUAL_OBJECTS_DIRECTORY) $(COREWAR_OBJECTS) $(VISUAL_OBJECTS)
	@$(CC) $(FLAGS) $(LIBRARIES) $(INCLUDES) $(COREWAR_OBJECTS) $(VISUAL_OBJECTS) -o $(COREWAR)
	@echo "$(BOLD_CYAN)$(COREWAR) $(GREEN)is compiled$(RESET)"

$(COREWAR_OBJECTS_DIRECTORY):
	@mkdir -p $(COREWAR_OBJECTS_DIRECTORY)

$(VISUAL_OBJECTS_DIRECTORY):
	@mkdir -p $(VISUAL_OBJECTS_DIRECTORY)

$(COREWAR_OBJECTS_DIRECTORY)%.o : $(COREWAR_SOURCES_DIRECTORY)%.c $(HEADERS)
	@echo "$(MAGENTA)Creating object file $(CYAN)$<$(RESET) $(MAGENTA)in $(COREWAR_OBJECTS_DIRECTORY)$(RESET)"
	@$(CC) $(FLAGS) -c $(INCLUDES)  $< -o $@

$(VISUAL_OBJECTS_DIRECTORY)%.o : $(VISUAL_SOURCES_DIRECTORY)%.c $(HEADERS)
	@echo "$(MAGENTA)Creating object file $(CYAN)$<$(RESET) $(MAGENTA)in $(VISUAL_OBJECTS_DIRECTORY)$(RESET)"
	@$(CC) $(FLAGS) -c $(INCLUDES)  $< -o $@

$(LIBFT):
	@cd $(LIBFT_DIRECTORY) && $(MAKE) -s

clean:
	@$(MAKE) -sC $(LIBFT_DIRECTORY) clean
	@rm -rf $(OBJECTS_DIRECTORY)
	@echo "$(GREEN)object files $(RED)deleted$(RESET)"
	@echo "$(GREEN)$(OBJECTS_DIRECTORY) $(RED)deleted$(RESET)"

fclean: clean
	@rm -f $(LIBFT)
	@echo "$(GREEN)$(LIBFT) $(RED)deleted$(RESET)"
	@rm -f $(COREWAR)
	@echo "$(GREEN)$(NAME) $(RED)deleted$(RESET)"

re:
	@$(MAKE) fclean
	@$(MAKE) all
