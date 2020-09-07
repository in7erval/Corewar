/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_list_add.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcaterpi <hcaterpi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/13 15:16:11 by hcaterpi          #+#    #+#             */
/*   Updated: 2019/09/14 15:41:38 by hcaterpi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list_gnl	*ft_list_add(t_list_gnl **begin_list, int fd, char *str)
{
	t_list_gnl	*current;

	if (!(*begin_list))
	{
		*begin_list = ft_list_create(fd, str);
		current = *begin_list;
	}
	else
	{
		current = *begin_list;
		while (current->next)
			current = current->next;
		current->next = ft_list_create(fd, str);
		current = current->next;
	}
	return (current);
}
