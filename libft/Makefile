# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ffood <ffood@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/09/06 18:03:43 by htrent            #+#    #+#              #
#    Updated: 2020/02/24 17:12:55 by ffood            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.PHONY: $(NAME) all compile fclean clean re

NAME = libft.a

FLAGS = -Wall -Werror -Wextra -c -g -O2

HEADERS_LIST = libft.h ft_printf.h
HEADERS_DIRECTORY = ./includes/
HEADERS = $(addprefix $(HEADERS_DIRECTORY), $(HEADERS_LIST))

SOURCES_DIRECTORY = ./
SOURCES_LIST = ft_atoi.c \
    ft_atoi_base.c \
    ft_atoi_pos.c \
    ft_bzero.c \
    ft_count_of_digits.c \
    ft_create_node.c \
    ft_isalnum.c \
    ft_isalpha.c \
    ft_isascii.c \
    ft_isdigit.c \
    ft_isinteger.c \
    ft_isprint.c \
    ft_istrchr.c \
    ft_itoa.c \
    ft_lstadd.c \
    ft_lstadd_back.c \
    ft_lstdel.c \
    ft_lstdelone.c \
    ft_lstiter.c \
    ft_lstmap.c \
    ft_lstnew.c \
    ft_lstrev.c \
    ft_memalloc.c \
    ft_memccpy.c \
    ft_memchr.c \
    ft_memcmp.c \
    ft_memcpy.c \
    ft_memdel.c \
    ft_memmove.c \
    ft_memset.c \
    ft_pow.c \
    ft_printf.c \
    ft_putchar.c \
    ft_putchar_buf.c \
    ft_putchar_fd.c \
    ft_putendl.c \
    ft_putendl_fd.c \
    ft_putnbr.c \
    ft_putnbr_fd.c \
    ft_putstr.c \
    ft_putstr_buf.c \
    ft_putstr_fd.c \
    ft_str_to_upper.c \
    ft_strcat.c \
    ft_strchr.c \
    ft_strclr.c \
    ft_strcmp.c \
    ft_strcpy.c \
    ft_strdel.c \
    ft_strdup.c \
    ft_strequ.c \
    ft_striter.c \
    ft_striteri.c \
    ft_strjoin.c \
    ft_strlcat.c \
    ft_strlen.c \
    ft_strmap.c \
    ft_strmapi.c \
    ft_strncat.c \
    ft_strncmp.c \
    ft_strncpy.c \
    ft_strnequ.c \
    ft_strnew.c \
    ft_strnstr.c \
    ft_strrchr.c \
    ft_strsplit.c \
    ft_freesplit.c \
    ft_strstr.c \
    ft_strsub.c \
    ft_strtrim.c \
    ft_tolower.c \
    ft_toupper.c \
    ft_utoa_base.c \
    ft_list_add.c \
    ft_list_create.c \
    ft_list_delete.c \
    ft_list_find.c \
    get_next_line.c \
    ft_printf/color.c \
    ft_printf/flags.c \
    ft_printf/float_fill.c \
    ft_printf/help_utils_b.c \
    ft_printf/help_utils_di.c \
    ft_printf/help_utils_o.c \
    ft_printf/help_utils_p.c \
    ft_printf/help_utils_s.c \
    ft_printf/help_utils_x.c \
    ft_printf/power_five.c \
    ft_printf/power_two.c \
    ft_printf/put_b.c \
    ft_printf/put_c.c \
    ft_printf/put_c_zero.c \
    ft_printf/put_data.c \
    ft_printf/put_di.c \
    ft_printf/put_exception.c \
    ft_printf/put_f.c \
    ft_printf/put_o.c \
    ft_printf/put_p.c \
    ft_printf/put_r.c \
    ft_printf/put_s.c \
    ft_printf/put_u.c \
    ft_printf/put_x.c \
    ft_printf/utils.c \
    ft_printf/width_prec_size.c

SOURCES = $(addprefix $(SOURCES_DIRECTORY), $(SOURCES_LIST))

OBJECTS_DIRECTORY = objects/
OBJECTS_DIRECTORY_PRINTF = objects/ft_printf
OBJECTS_LIST = $(patsubst %.c, %.o, $(SOURCES_LIST))
OBJECTS	= $(addprefix $(OBJECTS_DIRECTORY), $(OBJECTS_LIST))

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
CLEAR_LINE = \033[2K
BEGIN_LINE = \033[A

PWD = $(shell pwd)

TOTAL_FILES := $(shell echo $(SOURCES_LIST) | wc -w | sed -e 's/ //g')
CURRENT_FILES = $(shell find $(PWD)/objects/ -type f | wc -l | sed -e 's/ //g')

all: $(NAME)

$(NAME): $(OBJECTS_DIRECTORY) $(OBJECTS)
		@ar rc $(NAME) $(OBJECTS)
		@ranlib $(NAME)
		@echo "\n$(CLEAR_LINE)$(BOLD_YELLOW)$(NAME)$(RESET):$(BOLD_BLUE) ready to use!$(RESET)"

$(OBJECTS_DIRECTORY):
	@mkdir -p $(OBJECTS_DIRECTORY)
	@mkdir -p $(OBJECTS_DIRECTORY_PRINTF)
	@echo "$(BOLD_YELLOW)$(NAME)$(RESET): $(GREEN)$(OBJECTS_DIRECTORY) was created$(RESET)"

$(OBJECTS_DIRECTORY)%.o : $(SOURCES_DIRECTORY)%.c $(HEADERS)
	@gcc $(FLAGS) -I $(HEADERS_DIRECTORY) $< -o $@
	@echo "$(CLEAR_LINE)$(GREEN)Compiling file [$(CYAN)$<$(GREEN)]. ($(CURRENT_FILES) / $(TOTAL_FILES))$(RESET)$(BEGIN_LINE)"



clean:
	@rm -rf $(OBJECTS_DIRECTORY)
	@echo "$(BOLD_YELLOW)$(NAME)$(RESET): $(RED)$(OBJECTS_DIRECTORY) was deleted$(RESET)"
	@echo "$(BOLD_YELLOW)$(NAME)$(RESET): $(RED)object files were deleted$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(BOLD_RED)$(NAME) deleted$(RESET)"


re:
	@$(MAKE) fclean
	@$(MAKE) all
