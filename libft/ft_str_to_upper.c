/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str_to_upper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcaterpi <hcaterpi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/06 16:58:37 by htrent            #+#    #+#             */
/*   Updated: 2020/08/03 12:50:25 by hcaterpi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char	*ft_str_to_upper(char *str)
{
	int		i;
	char	c;

	if (!str)
		return (NULL);
	i = 0;
	while (str[i])
	{
		c = ft_toupper(str[i]);
		str[i] = c;
		i++;
	}
	return (str);
}
