/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/26 02:49:32 by majosue           #+#    #+#             */
/*   Updated: 2020/10/31 19:56:52 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

int				ft_get_op_code(const char *name)
{
	int i;

	i = 0;
	while (++i < 17)
	{
		if (ft_strequ(name, g_op_tab[i].name))
			return (i);
	}
	return (0);
}

unsigned int	ft_reverse_bytes(unsigned int value)
{
	return (value & 0x000000FFU) << 24 | (value & 0x0000FF00U) << 8 |
	(value & 0x00FF0000U) >> 8 | (value & 0xFF000000U) >> 24;
}

void			ft_write_acb(t_instruction *opr)
{
	int				i;
	unsigned char	acb;

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

void			ft_skip_args(int op, t_token **token, t_asm *assembler)
{
	int				i;
	t_instruction	opr;

	i = 0;
	ft_bzero(&opr, sizeof(opr));
	opr.byte_code[opr.pos] = op;
	opr.pos += 1 + g_op_tab[op].acb;
	while ((*token)->type != NEW_LINE)
	{
		ft_check_arg(i, &opr, *token, assembler);
		i++;
		*token = (*token)->next->type == SEPARATOR ?
			(*token)->next->next : (*token)->next;
	}
	if (i != g_op_tab[op].max_params)
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

void			ft_skip_operator(t_token **token, t_asm *assembler)
{
	int op;

	ft_skip_labels(token, assembler);
	if ((*token)->type == END)
		return ;
	if ((*token)->type != OPERATOR)
		ft_asm_exit(NULL, NULL, NULL, *token);
	if (!(op = ft_get_op_code((*token)->content)))
		ft_asm_exit(NULL, NULL, NULL, *token);
	*token = (*token)->next;
	ft_skip_args(op, token, assembler);
}
