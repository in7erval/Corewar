/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/17 07:44:40 by majosue           #+#    #+#             */
/*   Updated: 2020/10/24 14:54:10 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew2(void const *content, size_t content_size)
{
	t_list *new;

	if ((new = malloc(sizeof(*new))) == NULL)
		return (NULL);
	if (content == NULL)
	{
		new->content = NULL;
		new->content_size = 0;
		new->next = NULL;
		return (new);
	}
	if ((new->content = malloc(content_size)) == NULL)
	{
		free(new);
		new = (NULL);
		return (new);
	}
	new->content_size = content_size;
	ft_memmove(new->content, content, new->content_size);
	new->next = (NULL);
	return (new);
}
