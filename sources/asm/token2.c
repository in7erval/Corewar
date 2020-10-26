/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/26 02:57:56 by majosue           #+#    #+#             */
/*   Updated: 2020/10/26 04:44:31 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

t_token	*new_token(t_asm *assembler, int type)
{
	t_token *token;

	token = (t_token *)malloc(sizeof(t_token));
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

void	add_token(t_token **head, t_token *token)
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

void	parse_symbols(t_asm *assm, char *row,
	unsigned int start, t_token *token)
{
	unsigned int column;

	token->column = start;
	column = assm->column;
	while (row[assm->column] && ft_strchr(LABEL_CHARS, row[assm->column]))
		assm->column++;
	if (assm->column - column > 0 && row[assm->column] == LABEL_CHAR)
	{
		assm->column++;
		if (!(token->content = ft_strsub(row, start, assm->column - start)))
			ft_asm_exit(NULL, NULL, NULL, NULL);
		token->type = LABEL;
		add_token(&(assm->tokens), token);
	}
	else if (assm->column - column > 0 && is_delimiter(row[assm->column]))
	{
		if (!(token->content = ft_strsub(row, start, assm->column - start)))
			ft_asm_exit(NULL, NULL, NULL, NULL);
		if (token->type == INDIRECT)
			token->type = is_register(token->content) ? REGISTER : OPERATOR;
		add_token(&(assm->tokens), token);
	}
	else
		ft_asm_exit(NULL, (int[2]){assm->row, assm->column}, NULL, NULL);
}

void	parse_number(t_asm *assembler, char *row,
	unsigned int start, t_token *token)
{
	unsigned int column;

	token->column = start;
	if (row[assembler->column] == '-')
		assembler->column++;
	column = assembler->column;
	while (ft_isdigit(row[assembler->column]))
		assembler->column++;
	if (assembler->column - column > 0 && (token->type == DIRECT ||
	is_delimiter(row[assembler->column])))
	{
		if (!(token->content = ft_strsub(row, start,
		assembler->column - start)))
			ft_asm_exit(NULL, NULL, NULL, NULL);
		add_token(&(assembler->tokens), token);
	}
	else if (token->type != DIRECT)
	{
		assembler->column = start;
		parse_symbols(assembler, row, start, token);
	}
	else
		ft_asm_exit(NULL, (int[2]){assembler->row, assembler->column},
		NULL, NULL);
}

char	*ft_strjoin_extended(char **str1, char **str2)
{
	char *result;

	result = ft_strjoin(*str1, *str2);
	if (!result)
		ft_asm_exit(NULL, NULL, NULL, NULL);
	ft_strdel(str1);
	ft_strdel(str2);
	return (result);
}
