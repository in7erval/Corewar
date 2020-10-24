
#include "asm.h"

t_op g_op_tab[17] =
	{
		{0, 0, {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, 0, 0},
		{"live", 1, {{0, 0, T_DIR, 0}}, 1, 0},
		{"ld", 2, {{0, 0, T_DIR, T_IND}, {0, T_REG, 0, 0}}, 2, 1},
		{"st", 2, {{0, T_REG, 0, 0}, {0, T_REG, 0, T_IND}}, 3, 1},
		{"add", 3, {{0, T_REG}, {0, T_REG}, {0, T_REG}}, 4, 1},
		{"sub", 3, {{T_REG, T_REG, T_REG}}, 5, 1},
		{"and", 3, {{0, T_REG, T_DIR, T_IND}, {0, T_REG, T_DIR, T_IND}, {0, T_REG}}, 6, 1},
		{"or", 3, {{0, T_REG, T_DIR, T_IND}, {0, T_REG, T_DIR, T_IND}}, 7, 1},
		{"xor", 3, {{0, T_REG, T_DIR, T_IND}, {0, T_REG, T_DIR, T_IND}}, 8, 1},
		{"zjmp", 1, {{0, 0, T_DIR / 2}}, 9, 0},
		{"ldi", 3, {{0, T_REG, T_DIR / 2, T_IND}, {0, T_REG, T_DIR / 2}, {0, T_REG}}, 10, 1},
		{"sti", 3, {{0, T_REG}, {0, T_REG, T_DIR / 2, T_IND}, {0, T_REG, T_DIR / 2}}, 11, 1},
		{"fork", 1, {{0, 0, T_DIR / 2}}, 12, 0},
		{"lld", 2, {{0, 0, T_DIR, T_IND}, {0, T_REG}}, 13, 1},
		{"lldi", 3, {{0, T_REG, T_DIR / 2, T_IND}, {0, T_REG, T_DIR / 2}, {0, T_REG}}, 14, 1},
		{"lfork", 1, {{0, 0, T_DIR}}, 15, 0},
		{"aff", 1, {{0, T_REG}}, 16, 1}};

void print_usage(void)
{
	ft_printf("Usage: vm_champs/asm <sourcefile.s>\n");
	exit(0);
}

t_asm *init_asm(int fd)
{
	t_asm *assembler;

	assembler = (t_asm *)ft_memalloc(sizeof(t_asm)); //todo:free
	if (!assembler)
		ft_asm_exit(NULL, NULL, NULL, NULL);
	assembler->fd = fd;
	return (assembler);
}

t_token *new_token(t_asm *assembler, int type)
{
	t_token *token;

	token = (t_token *)malloc(sizeof(t_token)); //todo:free
	if (!token)
		ft_asm_exit(NULL, NULL, NULL, NULL);
	token->content = NULL;
	token->type = type;
	token->row = assembler->row;
	if (type == SEPARATOR || type == NEW_LINE)
		token->column = assembler->column - 1;
	else
		token->column = assembler->column;
	if (type == SEPARATOR)
	{
		if (!(token->content = ft_strnew(1)))
			ft_asm_exit(NULL, NULL, NULL, NULL);
		token->content[0] = SEPARATOR_CHAR;
	}
	token->next = NULL;
	return (token);
}

void add_token(t_token **head, t_token *token)
{
	t_token *temp;

	if (!head)
		return;
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

void parse_symbols(t_asm *assembler, char *row, unsigned int start, t_token *token)
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
			ft_asm_exit(NULL, NULL, NULL, NULL);
		token->type = LABEL;
		add_token(&(assembler->tokens), token);
	}
	else if (assembler->column - column > 0 && is_delimiter(row[assembler->column]))
	{
		if (!(token->content = ft_strsub(row, start, assembler->column - start)))
			ft_asm_exit(NULL, NULL, NULL, NULL);
		if (token->type == INDIRECT)
			token->type = is_register(token->content) ? REGISTER : OPERATOR;
		add_token(&(assembler->tokens), token);
	}
	else
		ft_asm_exit(NULL, (int[2]){assembler->row, assembler->column}, NULL, NULL);
}

void parse_number(t_asm *assembler, char *row, unsigned int start, t_token *token)
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
			ft_asm_exit(NULL, NULL, NULL, NULL);
		add_token(&(assembler->tokens), token);
	}
	else if (token->type != DIRECT)
	{
		assembler->column = start;
		parse_symbols(assembler, row, start, token);
	}
	else
		ft_asm_exit(NULL, (int[2]){assembler->row, assembler->column}, NULL, NULL);
}

char *ft_strjoin_extended(char **str1, char **str2)
{
	char *result;

	result = ft_strjoin(*str1, *str2);
	if (!result)
		ft_asm_exit(NULL, NULL, NULL, NULL);
	ft_strdel(str1);
	ft_strdel(str2);
	return (result);
}

void update_assembler_pos(t_asm *assembler, const char *str)
{
	unsigned int i;

	assembler->column++;
	i = assembler->column;
	while (str[i] && str[i] != '\"')
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

void reinit_row(char **row, char *str)
{
	char *s;

	s = ft_strdup(str);
	if (!s)
		ft_asm_exit(NULL, NULL, NULL, NULL);
	ft_strdel(row);
	*row = s;
}

void parse_string(t_asm *assembler, char **row, unsigned int start, t_token *token)
{
	int size;
	char *end;
	char *str;

	token->column = start;
	size = 1;
	while (!(end = ft_strchr(&((*row)[start + 1]), '\"')) && (size = read_row(assembler->fd, &str)) > 0)
		*row = ft_strjoin_extended(row, &str);
	update_assembler_pos(assembler, *row);
	if (size == -1)
		ft_asm_exit(NULL, NULL, NULL, NULL);
	if (size == 0)
		ft_asm_exit(NULL, (int[2]){assembler->row, assembler->column}, NULL, NULL);
	token->content = ft_strsub(*row, start, end + 1 - &((*row)[start]));
	if (!token->content)
		ft_asm_exit(NULL, NULL, NULL, NULL);
	if (end - assembler->column != *row)
		reinit_row(row, end - assembler->column);
	assembler->column++;
	add_token(&(assembler->tokens), token);
}

void parse_token2(t_asm *assembler, char **row)
{
	if ((*row)[assembler->column] == DIRECT_CHAR)
	{
		assembler->column++;
		if ((*row)[assembler->column] == LABEL_CHAR)
		{
			assembler->column++;
			parse_symbols(assembler, *row, assembler->column - 2,
						  new_token(assembler, DIRECT_LABEL));
		}
		else
			parse_number(assembler, *row, assembler->column - 1,
						 new_token(assembler, DIRECT));
	}
	else if ((*row)[assembler->column] == '\"')
		parse_string(assembler, row, assembler->column,
					 new_token(assembler, STRING));
	else if ((*row)[assembler->column] == LABEL_CHAR)
	{
		assembler->column++;
		parse_symbols(assembler, *row, assembler->column - 1,
					  new_token(assembler, INDIRECT_LABEL));
	}
	else
		parse_number(assembler, *row, assembler->column,
					 new_token(assembler, INDIRECT));
}

void parse_token(t_asm *assembler, char **row)
{
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
		parse_symbols(assembler, *row, assembler->column++,
					  new_token(assembler, COMMAND));
	else
		parse_token2(assembler, row);
}

void parse_file(t_asm *assembler)
{
	char *row;
	int ret;

	while ((ret = read_row(assembler->fd, &row)) > 0)
	{
		assembler->row++;
		assembler->column = 0;
		while (row[assembler->column])
		{
			skip_whitespaces(assembler, row);
			skip_comment(assembler, row);
			if (row[assembler->column])
				parse_token(assembler, &row);
		}
		ft_strdel(&row);
	}
	if (ret == -1)
		ft_asm_exit(NULL, NULL, NULL, NULL);
	add_token(&(assembler->tokens), new_token(assembler, END));
}

char *ft_get_token_name(int index, int upercase)
{
	char tokens[35][15];
	char *name;

	ft_bzero(tokens, 35 * 15);
	ft_strcpy(tokens[0x11], "command");
	ft_strcpy(tokens[0x12], "string");
	ft_strcpy(tokens[0x13], "label");
	ft_strcpy(tokens[0x14], "instruction");
	ft_strcpy(tokens[0x15], "register");
	ft_strcpy(tokens[0x16], "direct");
	ft_strcpy(tokens[0x17], "direct_label");
	ft_strcpy(tokens[0x18], "indirect");
	ft_strcpy(tokens[0x19], "indirect_label");
	ft_strcpy(tokens[0x20], "separator");
	ft_strcpy(tokens[0x21], "new_line");
	ft_strcpy(tokens[0x22], "end");
	if (!(name = ft_strdup(tokens[index])))
		ft_asm_exit(NULL, NULL, NULL, NULL);
	if (upercase)
		name = ft_str_to_upper(name);
	return (name);
}

void ft_invalid_label_exit(t_token *token)
{
	char *label_name;
	char *token_name;

	if (token->type == DIRECT_LABEL)
		label_name = token->content + 2;
	else
		label_name = token->content + 1;
	token_name = ft_get_token_name(token->type, 1);
	ft_printe("No such label %s while attempting to dereference token \
[TOKEN][%.3d:%.3d] %s \"%s\"\n",
			  label_name, token->row,
			  token->column + 1, token_name, token->content);
	ft_strdel(&token_name);
	exit(1);
}

void ft_asm_exit(char *str, int pos[2], int *i, t_token *token)
{
	char *token_name;

	if (token && i)
	{
		token_name = ft_get_token_name(token->type, 0);
		ft_printe("Invalid parameter %d type %s for instruction %s\n",
				  *i, token_name, str);
		ft_strdel(&token_name);
	}
	else if (token)
	{
		token_name = ft_get_token_name(token->type, 1);
		ft_printe("Syntax error at token [TOKEN][%.3d:%.3d] %s \"%s\"\n",
				  token->row, token->column + 1, token_name, token->content);
		ft_strdel(&token_name);
	}
	else if (pos)
		ft_printe("Lexical error at [%d:%d]\n", pos[0], pos[1]);
	else if (str)
		ft_printe("%s\n", str);
	else
		perror("ERROR");
	exit(1);
}

void ft_skip_name(t_token **token, t_asm *assembler, int *heat)
{
	*token = (*token)->next;
	if (ft_strlen((*token)->content) - 2 > PROG_NAME_LENGTH)
	{
		ft_printe("Champion name too long (Max length %d)\n",
				  PROG_NAME_LENGTH);
		exit(1);
	}
	ft_memmove(assembler->header.prog_name, (*token)->content + 1,
			   ft_strlen((*token)->content) - 2);
	*token = (*token)->next;
	if ((*token)->type != NEW_LINE)
		ft_asm_exit(NULL, NULL, NULL, *token);
	*token = (*token)->next;
	*heat = *heat | 1;
}

void ft_skip_comment(t_token **token, t_asm *assembler, int *heat)
{
	*token = (*token)->next;
	if (ft_strlen((*token)->content) - 2 > COMMENT_LENGTH)
	{
		ft_printe("Champion comment too long (Max length %d)\n",
				  COMMENT_LENGTH);
		exit(1);
	}
	ft_memmove(assembler->header.comment, (*token)->content + 1,
			   ft_strlen((*token)->content) - 2);
	*token = (*token)->next;
	if ((*token)->type != NEW_LINE)
		ft_asm_exit(NULL, NULL, NULL, *token);
	*token = (*token)->next;
	*heat = *heat | 2;
}

int ft_get_op_code(const char *name)
{
	int i = 0;

	while (++i < 17)
	{
		if (ft_strequ(name, g_op_tab[i].name))
			return (i);
	}
	return 0;
}

void ft_skip_name_or_comment(t_token **token, t_asm *assembler, int *heat)
{
	if ((*token)->type != COMMAND)
		ft_asm_exit(NULL, NULL, NULL, *token);
	else if ((*token)->next->type != STRING)
		ft_asm_exit(NULL, NULL, NULL, (*token)->next);
	if (ft_strequ((*token)->content, NAME_CMD_STRING) && !(*heat & 1))
		ft_skip_name(token, assembler, heat);
	else if (ft_strequ((*token)->content, COMMENT_CMD_STRING) && !(*heat & 2))
		ft_skip_comment(token, assembler, heat);
	else
		ft_asm_exit(NULL, NULL, NULL, *token);
}

void ft_skip_labels(t_token **token, t_asm *assembler)
{
	t_list *new_label_node;
	t_label *label_content;

	while ((*token)->type == LABEL)
	{
		if (!(label_content = (t_label *)ft_memalloc(sizeof(*label_content))))
			ft_asm_exit(NULL, NULL, NULL, NULL);
		label_content->label = (*token)->content;
		label_content->pos = assembler->pos;
		if (!(new_label_node = ft_lstnew(label_content, sizeof(*label_content))))
			ft_asm_exit(NULL, NULL, NULL, NULL);
		ft_lstadd_back(&assembler->labels, new_label_node);
		*token = (*token)->next->type == NEW_LINE ? (*token)->next->next : (*token)->next;
	}
}

int ft_get_arg_type_code(int type)
{
	if (type == REGISTER)
		return (REG_CODE);
	else if (type == DIRECT || type == DIRECT_LABEL)
		return (DIR_CODE);
	else if (type == INDIRECT || type == INDIRECT_LABEL)
		return (IND_CODE);
	return (0);
}

void ft_check_reg(t_instruction *opr, t_token *token)
{
	int reg_value;

	reg_value = ft_atoi(token->content + 1);
	if (reg_value <= 0 || reg_value > 99)
		ft_asm_exit(NULL, NULL, NULL, token);
	ft_memmove(&opr->byte_code[opr->pos], &reg_value, T_REG);
	opr->pos += T_REG;
}

unsigned int ft_reverse_bytes(unsigned int value)
{
	return (value & 0x000000FFU) << 24 | (value & 0x0000FF00U) << 8 |
		   (value & 0x00FF0000U) >> 8 | (value & 0xFF000000U) >> 24;
}

void ft_add_label_replace_point(t_token *token, t_instruction *opr, int size, t_asm *assembler)
{
	t_list *new_point;
	t_label_replace *new_point_content;

	if (!(new_point_content = (t_label_replace *)ft_memalloc(sizeof(*new_point_content))))
		ft_asm_exit(NULL, NULL, NULL, NULL);
	new_point_content->token = token;
	new_point_content->opr_pos = assembler->pos;
	new_point_content->insert_pos = opr->pos;
	new_point_content->size = size;
	if (!(new_point = ft_lstnew(new_point_content, sizeof(*new_point_content))))
		ft_asm_exit(NULL, NULL, NULL, NULL);
	ft_lstadd_back(&assembler->labels_replace_list, new_point);
}

void ft_check_dir(t_instruction *opr, t_token *token,
				  t_asm *assembler, int t_dir_size)
{
	int dir_value;

	if (token->type == DIRECT_LABEL)
		ft_add_label_replace_point(token,
								   opr, t_dir_size, assembler);
	else
	{
		dir_value = ft_atoi(token->content + 1);
		dir_value = dir_value << ((T_DIR - t_dir_size) * 8);
		dir_value = ft_reverse_bytes(dir_value);
		ft_memmove(&opr->byte_code[opr->pos], &dir_value, t_dir_size);
	}
	opr->pos += t_dir_size;
}

void ft_check_ind(t_instruction *opr, t_token *token, t_asm *assembler)
{
	int ind_value;

	if (token->type == INDIRECT_LABEL)
		ft_add_label_replace_point(token, opr, T_IND, assembler);
	else
	{
		ind_value = ft_atoi(token->content);
		ind_value = ind_value << 16;
		ind_value = ft_reverse_bytes(ind_value);
		ft_memmove(&opr->byte_code[opr->pos], &ind_value, T_IND);
	}
	opr->pos += T_IND;
}

void ft_check_arg(int i, t_instruction *opr, t_token *token, t_asm *assembler)
{

	if (!(opr->args[i] = ft_get_arg_type_code((token)->type)))
		ft_asm_exit(NULL, NULL, NULL, token);
	if (!g_op_tab[opr->byte_code[0]].types[i][opr->args[i]] ||
		i >= g_op_tab[opr->byte_code[0]].max_params)
		ft_asm_exit(g_op_tab[opr->byte_code[0]].name, NULL, &i, token);
	if (opr->args[i] == REG_CODE)
		ft_check_reg(opr, token);
	else if (opr->args[i] == DIR_CODE)
		ft_check_dir(opr, token, assembler,
					 g_op_tab[opr->byte_code[0]].types[i][opr->args[i]]);
	else
		ft_check_ind(opr, token, assembler);
}

void ft_write_acb(t_instruction *opr)
{
	int i;
	unsigned char acb;

	if (g_op_tab[opr->byte_code[0]].acb == 1)
	{
		i = 0;
		acb = 0;
		while (i < 3)
		{
			acb = acb | opr->args[i];
			acb = acb << 2;
			i++;
		}
		opr->byte_code[1] = acb;
	}
}

/*
**
*/

void ft_skip_args(int op, t_token **token, t_asm *assembler)
{
	int i;
	t_instruction opr;

	i = 0;
	ft_bzero(&opr, sizeof(opr));
	opr.byte_code[opr.pos] = op;
	opr.pos += 1 + g_op_tab[op].acb;
	while ((*token)->type != NEW_LINE)
	{
		ft_check_arg(i, &opr, *token, assembler);
		i++;
		*token = (*token)->next->type == SEPARATOR ? (*token)->next->next : (*token)->next;
	}
	if (i == 0)
		ft_asm_exit(NULL, NULL, NULL, *token);
	ft_write_acb(&opr);
	if (!(assembler->bytecode = realloc(assembler->bytecode,
										assembler->pos + opr.pos + 1)))
		ft_asm_exit(NULL, NULL, NULL, NULL);
	ft_memmove(&assembler->bytecode[assembler->pos],
			   opr.byte_code, opr.pos + 1);
	assembler->pos += opr.pos;
	*token = (*token)->next;
}

void ft_skip_operator(t_token **token, t_asm *assembler)
{
	int op;

	ft_skip_labels(token, assembler);
	if ((*token)->type == END)
		return;
	if ((*token)->type != OPERATOR)
		ft_asm_exit(NULL, NULL, NULL, *token);
	if (!(op = ft_get_op_code((*token)->content)))
		ft_asm_exit(NULL, NULL, NULL, *token);
	*token = (*token)->next;
	ft_skip_args(op, token, assembler);
}

void check_syntax(t_asm *assembler, t_token *tokens)
{
	int heat;

	heat = 0;
	ft_skip_name_or_comment(&tokens, assembler, &heat);
	ft_skip_name_or_comment(&tokens, assembler, &heat);
	while (tokens && tokens->type != END)
	{
		ft_skip_operator(&tokens, assembler);
	}
}

t_label *ft_get_label(t_token *token, t_asm *assembler)
{
	t_list *labels;
	t_label *label;
	size_t len;
	char *label_name;

	if (token->type == DIRECT_LABEL)
		label_name = token->content + 2;
	else
		label_name = token->content + 1;
	labels = assembler->labels;
	while (labels)
	{
		label = labels->content;
		len = ft_strlen(label->label);
		if (!(ft_strncmp(label->label, label_name, len - 1)) && label_name[len - 1] == '\0')
			return (label);
		labels = labels->next;
	}
	return (NULL);
}

void insert_labels_values(t_asm *assembler)
{
	int value;
	t_list *points;
	t_label_replace *point;
	t_label *label;

	points = assembler->labels_replace_list;
	while (points)
	{
		point = points->content;
		label = ft_get_label(point->token, assembler);
		if (!label)
			ft_invalid_label_exit(point->token);
		value = label->pos - point->opr_pos;
		value = value << ((REG_SIZE - point->size) * 8);
		value = ft_reverse_bytes(value);
		ft_memmove(&assembler->bytecode[point->opr_pos + point->insert_pos],
				   &value, point->size);
		points = points->next;
	}
}

void write_file(t_asm *assembler, char *name)
{
	char *output_name;
	char *tmp;
	int fd;

	if (!(output_name = ft_strsub(name, 0, ft_strlen(name) - 2)))
		ft_asm_exit(NULL, NULL, NULL, NULL);
	tmp = output_name;
	if (!(output_name = ft_strjoin(output_name, ".cor")))
		ft_asm_exit(NULL, NULL, NULL, NULL);
	ft_strdel(&tmp);
	if ((fd = open(output_name, O_WRONLY | O_CREAT, 0644)) < 0)
		ft_asm_exit(NULL, NULL, NULL, NULL);
	ft_printf("Writing output program to %s\n", output_name);
	ft_strdel(&output_name);
	assembler->header.magic = ft_reverse_bytes(COREWAR_EXEC_MAGIC);
	assembler->header.prog_size = ft_reverse_bytes(assembler->pos);
	if (write(fd, &assembler->header, sizeof(assembler->header)) < 0)
		ft_asm_exit(NULL, NULL, NULL, NULL);
	if (write(fd, assembler->bytecode, assembler->pos) < 0)
		ft_asm_exit(NULL, NULL, NULL, NULL);
	close(fd);
}

void	del(void *memory, size_t size)
{
	ft_bzero(memory, size);
	ft_memdel(&memory);
}

void ft_del_tokens(t_token **tokens)
{
	if (!*tokens)
		return;
	if ((*tokens)->next)
		ft_del_tokens(&(*tokens)->next);
	ft_strdel(&(*tokens)->content);
	ft_memdel((void**)tokens);
}

void clean_memory(t_asm *assembler)
{
	ft_lstdel(&assembler->labels, del);
	ft_lstdel(&assembler->labels_replace_list, del);
	ft_del_tokens(&assembler->tokens);
	free(assembler->bytecode);
	free(assembler);
}

int main(int argc, char **argv)
{
	int fd;
	t_asm *assembler;

	if (argc != 2)
		print_usage();
	fd = open(argv[1], O_RDONLY);
	if (fd <= 0)
		ft_asm_exit(NULL, NULL, NULL, NULL);
	assembler = init_asm(fd);
	parse_file(assembler);
	check_syntax(assembler, assembler->tokens);
	insert_labels_values(assembler);
	write_file(assembler, argv[1]);
	clean_memory(assembler);
	return (0);
}
