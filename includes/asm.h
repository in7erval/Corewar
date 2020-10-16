
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
	t_token				*tokens;
}						t_asm;


int 	read_row(int fd, char **row);

int 	is_whitespace(char c);
int		is_delimiter(char c);
int		is_register(const char *str);

void skip_whitespaces(t_asm *assembler, const char *row);
void skip_comment(t_asm *assembler, const char *row);

#endif //COREWAR_ASM_H
