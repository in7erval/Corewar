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

HEADERS_LIST = corewar.h corewar_vs.h op.h
HEADERS_LIST_ASM = asm.h op.h
HEADERS_DIRECTORY = ./includes/
HEADERS = $(addprefix $(HEADERS_DIRECTORY), $(HEADERS_LIST))
HEADERS_ASM = $(addprefix $(HEADERS_DIRECTORY), $(HEADERS_LIST_ASM))



SOURCES_DIRECTORY = ./sources/

ASM_SOURCES_DIRECTORY = $(SOURCES_DIRECTORY)asm/
ASM_SOURCES_LIST = asm.c \
					read_row.c \
					is.c \
					skip.c \
					asm_free_tools.c \
					ft_skip_name_or_comment.c \
					init.c \
					ft_check_arg.c \
					labels.c \
					syntax.c \
					token.c \
					token2.c

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
					free_tools.c \
					ft_read_params.c \
					ft_put_players_to_arena.c \
					init_and_usage.c \
					ft_duplicate_carriage.c

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

ASM_SOURCES = $(addprefix $(ASM_SOURCES_DIRECTORY), $(ASM_SOURCES_LIST))
VISUAL_SOURCES = $(addprefix $(VISUAL_SOURCES_DIRECTORY), $(VISUAL_SOURCES_LIST))
COREWAR_SOURCES = $(addprefix $(COREWAR_SOURCES_DIRECTORY), $(COREWAR_SOURCES_LIST))


OBJECTS_DIRECTORY = ./objects/

ASM_OBJECTS_DIRECTORY = $(OBJECTS_DIRECTORY)asm/
ASM_OBJECTS_LIST = $(patsubst %.c, %.o, $(ASM_SOURCES_LIST))
ASM_OBJECTS	= $(addprefix $(ASM_OBJECTS_DIRECTORY), $(ASM_OBJECTS_LIST))

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


all: $(COREWAR) $(ASM)
	@echo "$(BOLD_RED) Ready! $(RESET)"

$(ASM): $(LIBFT) $(ASM_OBJECTS_DIRECTORY) $(ASM_OBJECTS)
	@$(CC) $(FLAGS) $(LIBRARIES) $(INCLUDES) $(ASM_OBJECTS) -o $(ASM)
	@echo "$(BOLD_CYAN)$(ASM) $(GREEN)is compiled$(RESET)"

$(COREWAR): $(LIBFT) $(COREWAR_OBJECTS_DIRECTORY) $(VISUAL_OBJECTS_DIRECTORY) $(COREWAR_OBJECTS) $(VISUAL_OBJECTS)
	@$(CC) $(FLAGS) $(LIBRARIES) $(INCLUDES) $(COREWAR_OBJECTS) $(VISUAL_OBJECTS) -o $(COREWAR)
	@echo "$(BOLD_CYAN)$(COREWAR) $(GREEN)is compiled$(RESET)"

$(ASM_OBJECTS_DIRECTORY):
	@mkdir -p $(ASM_OBJECTS_DIRECTORY)

$(COREWAR_OBJECTS_DIRECTORY):
	@mkdir -p $(COREWAR_OBJECTS_DIRECTORY)

$(VISUAL_OBJECTS_DIRECTORY):
	@mkdir -p $(VISUAL_OBJECTS_DIRECTORY)

$(ASM_OBJECTS_DIRECTORY)%.o : $(ASM_SOURCES_DIRECTORY)%.c $(HEADERS_ASM)
	@echo "$(MAGENTA)Creating object file $(CYAN)$<$(RESET) $(MAGENTA)in $(ASM_OBJECTS_DIRECTORY)$(RESET)"
	@$(CC) $(FLAGS) -c $(INCLUDES)  $< -o $@

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
	@echo "$(GREEN)$(COREWAR) $(RED)deleted$(RESET)"
	@rm -f $(ASM)
	@echo "$(GREEN)$(ASM) $(RED)deleted$(RESET)"

re:
	@$(MAKE) fclean
	@$(MAKE) all
