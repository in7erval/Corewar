/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_pos.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcaterpi <hcaterpi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/03 17:02:57 by htrent            #+#    #+#             */
/*   Updated: 2020/08/03 12:49:49 by hcaterpi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int			ft_atoi_pos(const char *str)
{
	int	result;

	result = 0;
	while (*str && *str >= '0' && *str <= '9')
	{
		if (result * 10 + *str - '0' < result)
			return (0);
		result = result * 10 + *str - '0';
		str++;
	}
	return (result);
}
