
# include "asm.h"

void print_usage(void) {}

t_asm	*init_asm(int fd)
{
	t_asm *assembler;

	assembler = (t_asm *)malloc(sizeof(t_asm)); //todo:free
	if (!assembler)
		exit(0); //todo:add term
	assembler->fd = fd;
	assembler->row = 0;
	assembler->column = 0;
	assembler->tokens = NULL;
	return (assembler);
}

t_token		*new_token(t_asm *assembler, int type)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token)); //todo:free
	if (!token)
		exit(0); //todo:add term
	token->content = NULL;
	token->type = type;
	token->row = assembler->row;
	if (type == SEPARATOR || type == NEW_LINE)
		token->column = assembler->column - 1;
	else
		token->column = assembler->column;
	token->next = NULL;
	return (token);
}

void 	add_token(t_token **head, t_token *token)
{
	t_token *temp;

	if (!head)
		return ;
	if (*head)
	{
		temp = *head;
		while (temp->next)
			temp = temp->next;
		if (token->type == NEW_LINE && temp->type == NEW_LINE)
			free(token);
		else
			temp->next = token;
	}
	else
	{
		if (token->type == NEW_LINE)
			free(token);
		else
			*head = token;
	}
}

void 	parse_symbols(t_asm *assembler, char *row, unsigned int start, t_token *token)
{
	unsigned int column;

	token->column = start;
	column = assembler->column;
	while (row[assembler->column] && ft_strchr(LABEL_CHARS, row[assembler->column]))
		assembler->column++;
	if (assembler->column - column > 0 && row[assembler->column] == LABEL_CHAR)
	{
		assembler->column++;
		if (!(token->content = ft_strsub(row, start, assembler->column - start)))
			exit(0); //todo:add term
		token->type = LABEL;
		add_token(&(assembler->tokens), token);
	}
	else if (assembler->column - column > 0 && is_delimiter(row[assembler->column]))
	{
		if (!(token->content = ft_strsub(row, start, assembler->column - start)))
			exit(0); //todo:add term
		if (token->type == INDIRECT)
			token->type = is_register(token->content) ? REGISTER : OPERATOR;
		add_token(&(assembler->tokens), token);
	}
	else
		exit(0); //todo:add lexical error

}

void 	parse_number(t_asm *assembler, char *row, unsigned int start, t_token *token)
{
	unsigned int column;

	token->column = start;
	if (row[assembler->column] == '-')
		assembler->column++;
	column = assembler->column;
	while (ft_isdigit(row[assembler->column]))
		assembler->column++;
	if (assembler->column - column > 0 && (token->type == DIRECT || is_delimiter(row[assembler->column])))
	{
		if (!(token->content = ft_strsub(row, start, assembler->column - start)))
			exit(0); //todo:add term
		add_token(&(assembler->tokens), token);
	}
	else if (token->type != DIRECT)
	{
		assembler->column = start;
		parse_symbols(assembler, row, start, token);
	}
	else
		exit(0); //todo:add lexical error
}

char 	*ft_strjoin_extended(char **str1, char **str2)
{
	char *result;

	result = ft_strjoin(*str1, *str2);
	if (!result)
		exit(0); //todo:add terminate;
	ft_strdel(str1);
	ft_strdel(str2);
	return (result);
}

void 	update_assembler_pos(t_asm *assembler, const char *str)
{
	unsigned int i;

	assembler->column++;
	i = assembler->column;
	while (str[i] != '\"')
	{
		if (str[i] == '\n')
		{
			assembler->row++;
			assembler->column = 0;
		}
		else
			assembler->column++;
		i++;
	}
}

void	reinit_row(char **row, char *str)
{
	char *s;

	s = ft_strdup(str);
	if (!s)
		exit(0);//todo:add term;
	ft_strdel(row);
	*row = s;
}

void	parse_string(t_asm *assembler, char **row, unsigned int start, t_token *token)
{
	int		size;
	char	*end;
	char 	*str;

	token->column = start;
	size = 1;
	while (!(end = ft_strchr(&((*row)[start + 1]), '\"')) && (size = read_row(assembler->fd, &str)) > 0)
		*row = ft_strjoin_extended(row, &str);
	update_assembler_pos(assembler, *row);
	if (size == -1)
		exit(0); //todo:add terminate;
	if (size == 0)
		exit(0); //todo:add lexical error
	token->content = ft_strsub(*row, start, end + 1 - &((*row)[start]));
	if (!token->content)
		exit(0);//todo:add term
	if (end - assembler->column != *row)
		reinit_row(row, end - assembler->column);
	assembler->column++;
	add_token(&(assembler->tokens), token);
}

void 	parse_token(t_asm *assembler, char **row)
{
	//ft_printf("try to parse_token %d \'%c\' \'%s\'\n", assembler->column, (*row)[assembler->column], *row);
	if ((*row)[assembler->column] == SEPARATOR_CHAR)
	{
		assembler->column++;
		add_token(&(assembler->tokens), new_token(assembler, SEPARATOR));
	}
	else if ((*row)[assembler->column] == '\n')
	{
		assembler->column++;
		add_token(&(assembler->tokens), new_token(assembler, NEW_LINE));
	}
	else if ((*row)[assembler->column] == '.')
	{
		//ft_printf("command\n");
		parse_symbols(assembler, *row, assembler->column++, new_token(assembler, COMMAND));
	}
	else if ((*row)[assembler->column] == DIRECT_CHAR)
	{
		assembler->column++;
		if ((*row)[assembler->column] == LABEL_CHAR)
		{
			assembler->column++;
			parse_symbols(assembler, *row, assembler->column - 2, new_token(assembler, DIRECT_LABEL));
		}
		else
			parse_number(assembler, *row, assembler->column - 1, new_token(assembler, DIRECT));
	}
	else if ((*row)[assembler->column] == '\"')
	{
		//ft_printf("parse string\n");
		parse_string(assembler, row, assembler->column, new_token(assembler, STRING));
	}
	else if ((*row)[assembler->column] == LABEL_CHAR)
	{
		assembler->column++;
		parse_symbols(assembler, *row, assembler->column - 2, new_token(assembler, INDIRECT_LABEL));
	}
	else
		parse_number(assembler, *row, assembler->column, new_token(assembler, INDIRECT));

}

void 	print_tokens(t_token *tokens)
{
	while (tokens)
	{
		ft_printf("\'{red}%20s{eoc}\' row:{cyan}%2d{eoc} column:{cyan}%2d{eoc} type:{green}",
				  tokens->content ? tokens->content : "", tokens->row, tokens->column);
		if (tokens->type == COMMAND)
			ft_printf("command");
		else if (tokens->type == STRING)
			ft_printf("string");
		else if (tokens->type == LABEL)
			ft_printf("label");
		else if (tokens->type == OPERATOR)
			ft_printf("operator");
		else if (tokens->type == REGISTER)
			ft_printf("register");
		else if (tokens->type == DIRECT)
			ft_printf("direct");
		else if (tokens->type == DIRECT_LABEL)
			ft_printf("direct_label");
		else if (tokens->type == INDIRECT)
			ft_printf("inderect_label");
		else if (tokens->type == SEPARATOR)
			ft_printf("separator");
		else if (tokens->type == NEW_LINE)
			ft_printf("new_line");
		else if (tokens->type == END)
			ft_printf("end");
		ft_printf("{eoc}\n");
		tokens = tokens->next;
	}
}

void 	parse_file(t_asm *assembler)
{
	char	*row;
	int 	ret;

	while ((ret = read_row(assembler->fd, &row)) > 0)
	{
		//ft_printf("line read\n");
		assembler->row++;
		assembler->column = 0;
		while (row[assembler->column])
		{
			skip_whitespaces(assembler, row);
			skip_comment(assembler, row);
			//ft_printf("ready to parse\n");
			if (row[assembler->column])
				parse_token(assembler, &row);
			//print_tokens(assembler->tokens);
		}
		ft_strdel(&row);
	}
	if (ret == -1)
		exit(0);//todo:add term;
	add_token(&(assembler->tokens), new_token(assembler, END));
}

int main(int argc, char **argv)
{
	int		fd;
	t_asm	*assembler;

	if (argc != 2)
		print_usage();
	fd = open(argv[1], O_RDONLY);
	if (fd <= 0)
		exit(0); //todo:add terminate program
	assembler = init_asm(fd);
	parse_file(assembler);
	print_tokens(assembler->tokens);
	return (0);
}

