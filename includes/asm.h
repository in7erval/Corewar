
#ifndef COREWAR_ASM_H
#define COREWAR_ASM_H

# include "libft.h"
# include "ft_printf.h"
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

typedef struct 			s_token
{
	char 				*content;
	unsigned int 		row;
	unsigned int 		column;
	int					type;
	struct s_token		*next;
}						t_token;

typedef struct			s_asm
{
	int 				fd;
	unsigned int		row;
	unsigned int		column;
	t_header			header;
	unsigned char		*bytecode;
	unsigned int		pos;
	t_list				*labels;
	t_token				*tokens;
}						t_asm;

typedef struct			s_label
{
	char				*label;
	unsigned int		pos;
}						t_label;

typedef struct			s_op
{
	char		*name;
	int			max_params;
	int			types[3][4];
	int			op_code;
	int			acb;
}						t_op;

struct					s_byte
{
	unsigned at2: 2;
	unsigned at4: 2;
	unsigned at6: 2;
	unsigned at8: 2;
};

typedef union			u_byte
{
	unsigned char	byte;
	struct s_byte	value;
}						t_arg_byte;

int 	read_row(int fd, char **row);

int 	is_whitespace(char c);
int		is_delimiter(char c);
int		is_register(const char *str);

void skip_whitespaces(t_asm *assembler, const char *row);
void skip_comment(t_asm *assembler, const char *row);

#endif //COREWAR_ASM_H
