/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_prec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/29 12:29:15 by majosue           #+#    #+#             */
/*   Updated: 2020/10/24 14:04:16 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printe.h"

/*
** Set precision in p and return 1 if it exist
*/

int	ft_prec(char *str, size_t *p)
{
	char	*point;
	int		present;

	present = 0;
	if ((point = ft_strchr(str, '.')))
	{
		present = 1;
		*p = ft_atoi(point + 1);
	}
	return (present);
}
