/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm_free_tools.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/26 01:53:08 by majosue           #+#    #+#             */
/*   Updated: 2020/10/26 05:20:58 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

void	ft_invalid_label_exit(t_token *token)
{
	char *label_name;
	char *token_name;

	if (token->type == DIRECT_LABEL)
		label_name = token->content + 2;
	else
		label_name = token->content + 1;
	token_name = ft_get_token_name(token->type, 1);
	ft_printe("No such label %s while attempting to dereference token \
[TOKEN][%.3d:%.3d] %s \"%s\"\n", label_name, token->row,
								token->column + 1, token_name, token->content);
	ft_strdel(&token_name);
	exit(1);
}

void	ft_asm_exit(char *str, int pos[2], int *i, t_token *token)
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
		ft_printe("Lexical error at [%d:%d]\n", pos[0], pos[1] + 1);
	else if (str)
		ft_printe("%s\n", str);
	else
		perror("ERROR");
	exit(1);
}

void	del(void *memory, size_t size)
{
	ft_bzero(memory, size);
	ft_memdel(&memory);
}

void	ft_del_tokens(t_token **tokens)
{
	if (!*tokens)
		return ;
	if ((*tokens)->next)
		ft_del_tokens(&(*tokens)->next);
	ft_strdel(&(*tokens)->content);
	ft_memdel((void **)tokens);
}

void	clean_memory(t_asm *assembler)
{
	ft_lstdel(&assembler->labels, del);
	ft_lstdel(&assembler->labels_replace_list, del);
	ft_del_tokens(&assembler->tokens);
	free(assembler->bytecode);
	free(assembler);
}
