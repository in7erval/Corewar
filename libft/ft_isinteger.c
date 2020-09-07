/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isinteger.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcaterpi <hcaterpi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/08 11:21:43 by hcaterpi          #+#    #+#             */
/*   Updated: 2020/02/12 15:15:51 by hcaterpi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_isinteger(const char *str)
{
	char	*buffer;
	int		index;

	buffer = (char *)str;
	index = 0;
	if ((*buffer == '-' || *buffer == '+') && (index = 1))
		buffer++;
	while (*buffer == '0' && *(buffer + 1) && ++index)
		buffer++;
	while (*buffer)
	{
		if (*buffer < '0' || *buffer > '9')
			return (0);
		buffer++;
	}
	buffer = (char *)str;
	if (ft_strlen(&buffer[index]) > 10 || ft_strlen(&buffer[index]) == 0
	|| (ft_strlen(&buffer[index]) == 10
	&& ((buffer[0] == '-' && ft_strcmp(&buffer[index], "2147483648") > 0)
	|| ((buffer[0] != '-' && ft_strcmp(&buffer[index], "2147483647") > 0)))))
		return (0);
	return (1);
}
