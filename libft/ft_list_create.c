/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_list_create.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcaterpi <hcaterpi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/13 15:12:35 by hcaterpi          #+#    #+#             */
/*   Updated: 2019/09/14 14:37:20 by hcaterpi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list_gnl	*ft_list_create(int fd, char *str)
{
	t_list_gnl	*current;

	current = malloc(sizeof(t_list_gnl));
	if (current)
	{
		current->fd = fd;
		current->str = str;
		current->next = NULL;
	}
	return (current);
}
