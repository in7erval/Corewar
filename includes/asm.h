/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/26 01:58:24 by htrent            #+#    #+#             */
/*   Updated: 2020/10/26 05:28:54 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COREWAR_ASM_H
# define COREWAR_ASM_H

# include "libft.h"
# include "ft_printf.h"
# include "ft_printe.h"
# include "op.h"

# define COMMAND		0x11
# define STRING			0x12
# define LABEL			0x13
# define OPERATOR		0x14
# define REGISTER		0x15
# define DIRECT			0x16
# define DIRECT_LABEL 	0x17
# define INDIRECT 		0x18
# define INDIRECT_LABEL 0x19
# define SEPARATOR		0x20
# define NEW_LINE 		0x21
# define END 			0x22

typedef struct			s_token
{
	char				*content;
	unsigned int		row;
	unsigned int		column;
	int					type;
	struct s_token		*next;
}						t_token;

typedef struct			s_asm
{
	int					fd;
	unsigned int		row;
	unsigned int		column;
	t_header			header;
	unsigned char		*bytecode;
	unsigned int		pos;
	t_list				*labels;
	t_list				*labels_replace_list;
	t_token				*tokens;
}						t_asm;

typedef struct			s_label
{
	char				*label;
	unsigned int		pos;
}						t_label;

typedef struct			s_rlabel_replace
{
	t_token				*token;
	int					opr_pos;
	int					insert_pos;
	int					size;
}						t_label_replace;

typedef struct			s_instruction
{
	int					pos;
	unsigned char		args[3];
	unsigned char		byte_code[32];
}						t_instruction;

typedef struct			s_op
{
	char				*name;
	int					max_params;
	int					types[3][4];
	int					op_code;
	int					acb;
}						t_op;

extern t_op				g_op_tab[17];

int						read_row(int fd, char **row);
int						is_whitespace(char c);
int						is_delimiter(char c);
int						is_register(const char *str);
void					skip_whitespaces(t_asm *assembler, const char *row);
void					skip_comment(t_asm *assembler, const char *row);
void					ft_asm_exit(char *str, int pos[2],
	int *i, t_token *token);
void					clean_memory(t_asm *assembler);
void					ft_invalid_label_exit(t_token *token);
char					*ft_get_token_name(int index, int upercase);
void					ft_skip_name_or_comment(t_token **token,
	t_asm *assembler, int *heat);
void					parse_string(t_asm *assembler, char **row,
	unsigned int start, t_token *token);
void					add_token(t_token **head, t_token *token);
void					ft_skip_operator(t_token **token, t_asm *assembler);
unsigned int			ft_reverse_bytes(unsigned int value);
void					print_usage(void);
t_asm					*init_asm(int fd);
void					insert_labels_values(t_asm *assembler);
void					parse_token(t_asm *assembler, char **row);
void					add_token(t_token **head, t_token *token);
t_token					*new_token(t_asm *assembler, int type);
void					ft_add_label_replace_point(t_token *token,
	t_instruction *opr, int size, t_asm *assembler);
void					ft_skip_labels(t_token **token, t_asm *assembler);
void					ft_check_arg(int i, t_instruction *opr,
	t_token *token, t_asm *assembler);
char					*ft_strjoin_extended(char **str1, char **str2);
void					parse_number(t_asm *assembler, char *row,
	unsigned int start, t_token *token);
void					parse_symbols(t_asm *assm, char *row,
	unsigned int start, t_token *token);

#endif
