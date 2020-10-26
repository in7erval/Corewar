/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_arg.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/26 02:21:19 by majosue           #+#    #+#             */
/*   Updated: 2020/10/26 05:18:58 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

int		ft_get_arg_type_code(int type)
{
	if (type == REGISTER)
		return (REG_CODE);
	else if (type == DIRECT || type == DIRECT_LABEL)
		return (DIR_CODE);
	else if (type == INDIRECT || type == INDIRECT_LABEL)
		return (IND_CODE);
	return (0);
}

void	ft_check_reg(t_instruction *opr, t_token *token)
{
	int reg_value;

	reg_value = ft_atoi(token->content + 1);
	if (reg_value <= 0 || reg_value > 99)
		ft_asm_exit(NULL, NULL, NULL, token);
	ft_memmove(&opr->byte_code[opr->pos], &reg_value, T_REG);
	opr->pos += T_REG;
}

void	ft_check_dir(t_instruction *opr, t_token *token,
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

void	ft_check_ind(t_instruction *opr, t_token *token, t_asm *assembler)
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

void	ft_check_arg(int i, t_instruction *opr,
											t_token *token, t_asm *assembler)
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
