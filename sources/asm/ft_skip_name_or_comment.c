/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_skip_name_or_comment.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/26 02:10:54 by majosue           #+#    #+#             */
/*   Updated: 2020/10/26 05:16:37 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

char	*ft_get_token_name(int index, int upercase)
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

void	ft_skip_name(t_token **token, t_asm *assembler, int *heat)
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

void	ft_skip_comment(t_token **token, t_asm *assembler, int *heat)
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

void	ft_skip_name_or_comment(t_token **token, t_asm *assembler, int *heat)
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
