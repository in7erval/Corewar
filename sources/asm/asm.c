/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/26 01:41:42 by majosue           #+#    #+#             */
/*   Updated: 2020/11/08 15:13:54 by htrent           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

t_op	g_op_tab[17] =
{
	{0, 0, {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, 0, 0},
	{"live", 1, {{0, 0, T_DIR, 0}}, 1, 0},
	{"ld", 2, {{0, 0, T_DIR, T_IND}, {0, T_REG, 0, 0}}, 2, 1},
	{"st", 2, {{0, T_REG, 0, 0}, {0, T_REG, 0, T_IND}}, 3, 1},
	{"add", 3, {{0, T_REG}, {0, T_REG}, {0, T_REG}}, 4, 1},
	{"sub", 3, {{0, T_REG}, {0, T_REG}, {0, T_REG}}, 5, 1},
	{"and", 3, {{0, T_REG, T_DIR, T_IND},
			{0, T_REG, T_DIR, T_IND}, {0, T_REG}}, 6, 1},
	{"or", 3, {{0, T_REG, T_DIR, T_IND},
			{0, T_REG, T_DIR, T_IND}, {0, T_REG}}, 7, 1},
	{"xor", 3, {{0, T_REG, T_DIR, T_IND},
			{0, T_REG, T_DIR, T_IND}, {0, T_REG}}, 8, 1},
	{"zjmp", 1, {{0, 0, T_DIR / 2}}, 9, 0},
	{"ldi", 3, {{0, T_REG, T_DIR / 2, T_IND},
			{0, T_REG, T_DIR / 2}, {0, T_REG}}, 10, 1},
	{"sti", 3, {{0, T_REG}, {0, T_REG, T_DIR / 2, T_IND},
			{0, T_REG, T_DIR / 2}}, 11, 1},
	{"fork", 1, {{0, 0, T_DIR / 2}}, 12, 0},
	{"lld", 2, {{0, 0, T_DIR, T_IND}, {0, T_REG}}, 13, 1},
	{"lldi", 3, {{0, T_REG, T_DIR / 2, T_IND},
			{0, T_REG, T_DIR / 2}, {0, T_REG}}, 14, 1},
	{"lfork", 1, {{0, 0, T_DIR / 2}}, 15, 0},
	{"aff", 1, {{0, T_REG}}, 16, 1}
};

void	parse_file(t_asm *assembler)
{
	char	*row;
	int		ret;

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

void	check_syntax(t_asm *assembler, t_token *tokens)
{
	int heat;

	heat = 0;
	ft_skip_name_or_comment(&tokens, assembler, &heat);
	ft_skip_name_or_comment(&tokens, assembler, &heat);
	while (tokens && tokens->type != END)
	{
		ft_skip_operator(&tokens, assembler);
	}
	if (!assembler->pos)
	{
		ft_printe("ERROR: no champion code found\n");
		exit(1);
	}
}

void	write_file(t_asm *assembler, char *name)
{
	char	*output_name;
	char	*tmp;
	int		fd;

	if (!(output_name = ft_strsub(name, 0, ft_strlen(name) - 2)))
		ft_asm_exit(NULL, NULL, NULL, NULL);
	tmp = output_name;
	if (!(output_name = ft_strjoin(output_name, ".cor")))
		ft_asm_exit(NULL, NULL, NULL, NULL);
	ft_strdel(&tmp);
	if ((fd = open(output_name, O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0)
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

int		main(int argc, char **argv)
{
	int		fd;
	t_asm	*assembler;
	int		len;

	if (argc != 2 || (len = ft_strlen(argv[1])) < 2 ||
	ft_strcmp(&argv[1][len - 2], ".s"))
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
	close(fd);
	return (0);
}
