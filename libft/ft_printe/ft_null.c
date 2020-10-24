/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_null.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/29 11:44:57 by majosue           #+#    #+#             */
/*   Updated: 2020/10/24 14:03:48 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printe.h"

/*
** Find '0' in flags
*/

int	ft_null(char *str)
{
	char *flags[5];
	char *str2;

	str2 = str;
	ft_gettab(&flags, 0);
	ft_chkflags(&str, flags);
	if (ft_memchr(str2, '0', str - str2))
		return (1);
	return (0);
}
