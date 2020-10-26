/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_row.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/26 05:03:58 by htrent            #+#    #+#             */
/*   Updated: 2020/10/26 05:07:29 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

static char	*ft_strchr_extended(const char *s, int c)
{
	if (s == NULL)
		return (NULL);
	return (ft_strchr(s, c));
}

static int	div_str(char **str, char **row)
{
	char *new;
	char *div;

	div = ft_strchr_extended(*str, '\n');
	div++;
	*row = ft_strsub(*str, 0, div - *str);
	if (*row == NULL)
		return (-1);
	if (ft_strlen(div) == 0)
	{
		free(*str);
		*str = NULL;
		return (1);
	}
	new = ft_strdup(div);
	free(*str);
	*str = new;
	return (new ? 1 : -1);
}

int			read_row(int fd, char **row)
{
	static char	*str = NULL;
	char		buf[BUFF_SIZE + 1];
	size_t		size;
	char		*tmp;

	if (fd < 0 || row == NULL || read(fd, buf, 0) < 0)
		return (-1);
	while (ft_strchr_extended(str, '\n') == NULL)
	{
		if (!(size = read(fd, buf, BUFF_SIZE)))
		{
			*row = str;
			if (str == NULL)
				return (0);
			str = NULL;
			return (1);
		}
		buf[size] = '\0';
		tmp = str;
		str = ft_strjoin(str, buf);
		free(tmp);
		if (!str)
			return (-1);
	}
	return (div_str(&str, row));
}
