/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_list_find.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcaterpi <hcaterpi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/13 16:34:13 by hcaterpi          #+#    #+#             */
/*   Updated: 2019/09/15 14:35:25 by hcaterpi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list_gnl	*ft_list_find(t_list_gnl *begin_list, int fd)
{
	t_list_gnl	*current;

	current = begin_list;
	while (current)
	{
		if (current->fd == fd)
			return (current);
		current = current->next;
	}
	return (NULL);
}
