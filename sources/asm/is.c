//
// Created by Дмитрий Юдаков on 16.10.2020.
//
# include "asm.h"

int 	is_whitespace(char c)
{
	return (c == '\t' || c == ' ' || c == '\v' || c == '\f' || c == '\r');
}

int		is_delimiter(char c)
{
	return (c == '\n' || c == '\"' || c == 0 || is_whitespace(c)
			|| c == COMMENT_CHAR || c == '.' || c == SEPARATOR_CHAR
			|| c == DIRECT_CHAR);
}

int		is_register(const char *str)
{
	int i;
	int len;

	len = ft_strlen(str);
	i = 0;
	if (len >= 2 && len <= 3 && str[i] == 'r')
	{
		i++;
		while (ft_isdigit(str[i]))
			i++;
		return (str[i] == 0 && ft_atoi(&str[1]) > 0);
	}
	return (0);
}