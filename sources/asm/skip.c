//
// Created by Дмитрий Юдаков on 16.10.2020.
//
# include "asm.h"

void skip_whitespaces(t_asm *assembler, const char *row)
{
	while (is_whitespace(row[assembler->column]))
		assembler->column++;
}

void skip_comment(t_asm *assembler, const char *row)
{
	if (row[assembler->column] == COMMENT_CHAR)
		while (row[assembler->column] != '\n')
			assembler->column++;
}

