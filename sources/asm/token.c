/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/26 02:53:14 by majosue           #+#    #+#             */
/*   Updated: 2020/10/26 04:54:38 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

void	update_assembler_pos(t_asm *assembler, const char *str)
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

void	reinit_row(char **row, char *str)
{
	char *s;

	s = ft_strdup(str);
	if (!s)
		ft_asm_exit(NULL, NULL, NULL, NULL);
	ft_strdel(row);
	*row = s;
}

void	parse_string(t_asm *assembler,
	char **row, unsigned int start, t_token *token)
{
	int		size;
	char	*end;
	char	*str;

	token->column = start;
	size = 1;
	while (!(end = ft_strchr(&((*row)[start + 1]), '\"')) &&
				(size = read_row(assembler->fd, &str)) > 0)
		*row = ft_strjoin_extended(row, &str);
	update_assembler_pos(assembler, *row);
	if (size == -1)
		ft_asm_exit(NULL, NULL, NULL, NULL);
	if (size == 0)
		ft_asm_exit(NULL, (int[2]){assembler->row, assembler->column},
														NULL, NULL);
	token->content = ft_strsub(*row, start, end + 1 - &((*row)[start]));
	if (!token->content)
		ft_asm_exit(NULL, NULL, NULL, NULL);
	if (end - assembler->column != *row)
		reinit_row(row, end - assembler->column);
	assembler->column++;
	add_token(&(assembler->tokens), token);
}

void	parse_token2(t_asm *assembler, char **row)
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

void	parse_token(t_asm *assembler, char **row)
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
