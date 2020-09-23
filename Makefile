.PHONY: all clean fclean re

NAME = corewar
CC = gcc
FLAGS = -Wall -Werror -Wextra -g
LIBRARIES = -lft -L$(LIBFT_DIRECTORY)

INCLUDES = -I$(HEADERS_DIRECTORY) -I$(LIBFT_HEADERS)

LIBFT = $(LIBFT_DIRECTORY)libft.a
LIBFT_DIRECTORY = ./libft/
LIBFT_HEADERS = $(LIBFT_DIRECTORY)includes/

HEADERS_LIST = corewar.h
HEADERS_DIRECTORY = ./includes/
HEADERS = $(addprefix $(HEADERS_DIRECTORY), $(HEADERS_LIST))



SOURCES_DIRECTORY = ./sources/

SOURCES_COREWAR_LIST = main.c


SOURCES_COREWAR = $(addprefix $(SOURCES_DIRECTORY), $(SOURCES_COREWAR_LIST))


OBJECTS_DIRECTORY = objects/
OBJECTS_COREWAR_LIST = $(patsubst %.c, %.o, $(SOURCES_COREWAR_LIST))
OBJECTS_COREWAR	= $(addprefix $(OBJECTS_DIRECTORY), $(OBJECTS_COREWAR_LIST))

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


all: $(NAME)
	@echo "$(BOLD_RED) Ready! $(RESET)"

$(NAME): $(LIBFT) $(OBJECTS_DIRECTORY) $(OBJECTS) $(OBJECTS_COREWAR)
	@$(CC) $(FLAGS) $(LIBRARIES) $(INCLUDES) $(OBJECTS_COREWAR) -o $(NAME)
	@echo "$(BOLD_CYAN)$(NAME) $(GREEN)is compiled$(RESET)"

$(OBJECTS_DIRECTORY)%.o : $(SOURCES_DIRECTORY)%.c $(HEADERS)
	@echo "$(MAGENTA)Creating object file $(CYAN)$<$(RESET) $(MAGENTA)in $(OBJECTS_DIRECTORY)$(RESET)"
	@$(CC) $(FLAGS) -c $(INCLUDES)  $< -o $@

$(OBJECTS_DIRECTORY):
	@mkdir -p $(OBJECTS_DIRECTORY)

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
	@rm -f $(NAME)
	@echo "$(GREEN)$(NAME) $(RED)deleted$(RESET)"

re:
	@$(MAKE) fclean
	@$(MAKE) all
