/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_list_delete.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcaterpi <hcaterpi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/13 15:16:11 by hcaterpi          #+#    #+#             */
/*   Updated: 2019/09/16 16:04:20 by hcaterpi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_list_delete(t_list_gnl **begin_list, int fd)
{
	t_list_gnl	*current;
	t_list_gnl	*previous;

	current = *begin_list;
	previous = current;
	while (current)
	{
		if (current->fd == fd)
		{
			if (previous == current)
				*begin_list = current->next;
			else
				previous->next = current->next;
			if (current->str)
				free(current->str);
			free(current);
			return (0);
		}
		previous = current;
		current = current->next;
	}
	return (1);
}
