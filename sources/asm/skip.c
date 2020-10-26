/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skip.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/26 05:02:30 by htrent            #+#    #+#             */
/*   Updated: 2020/10/26 05:03:42 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

void	skip_whitespaces(t_asm *assembler, const char *row)
{
	while (is_whitespace(row[assembler->column]))
		assembler->column++;
}

void	skip_comment(t_asm *assembler, const char *row)
{
	if (row[assembler->column] == COMMENT_CHAR)
		while (row[assembler->column] != '\n')
			assembler->column++;
}
