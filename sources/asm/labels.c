/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   labels.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/26 02:17:33 by majosue           #+#    #+#             */
/*   Updated: 2020/10/26 05:11:52 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

void	ft_skip_labels(t_token **token, t_asm *assembler)
{
	t_list	*new_label_node;
	t_label	*label_content;

	while ((*token)->type == LABEL)
	{
		if (!(label_content = (t_label *)ft_memalloc(sizeof(*label_content))))
			ft_asm_exit(NULL, NULL, NULL, NULL);
		label_content->label = (*token)->content;
		label_content->pos = assembler->pos;
		if (!(new_label_node = ft_lstnew(label_content,
													sizeof(*label_content))))
			ft_asm_exit(NULL, NULL, NULL, NULL);
		ft_lstadd_back(&assembler->labels, new_label_node);
		*token = (*token)->next->type == NEW_LINE ?
										(*token)->next->next : (*token)->next;
	}
}

void	ft_add_label_replace_point(t_token *token,
							t_instruction *opr, int size, t_asm *assembler)
{
	t_list			*new_point;
	t_label_replace	*new_point_content;

	if (!(new_point_content =
			(t_label_replace *)ft_memalloc(sizeof(*new_point_content))))
		ft_asm_exit(NULL, NULL, NULL, NULL);
	new_point_content->token = token;
	new_point_content->opr_pos = assembler->pos;
	new_point_content->insert_pos = opr->pos;
	new_point_content->size = size;
	if (!(new_point = ft_lstnew(new_point_content,
												sizeof(*new_point_content))))
		ft_asm_exit(NULL, NULL, NULL, NULL);
	ft_lstadd_back(&assembler->labels_replace_list, new_point);
}

t_label	*ft_get_label(t_token *token, t_asm *assembler)
{
	t_list	*labels;
	t_label	*label;
	size_t	len;
	char	*label_name;

	if (token->type == DIRECT_LABEL)
		label_name = token->content + 2;
	else
		label_name = token->content + 1;
	labels = assembler->labels;
	while (labels)
	{
		label = labels->content;
		len = ft_strlen(label->label);
		if (!(ft_strncmp(label->label, label_name,
									len - 1)) && label_name[len - 1] == '\0')
			return (label);
		labels = labels->next;
	}
	return (NULL);
}

void	insert_labels_values(t_asm *assembler)
{
	int				value;
	t_list			*points;
	t_label_replace	*point;
	t_label			*label;

	points = assembler->labels_replace_list;
	while (points)
	{
		point = points->content;
		label = ft_get_label(point->token, assembler);
		if (!label)
			ft_invalid_label_exit(point->token);
		value = label->pos - point->opr_pos;
		value = value << ((REG_SIZE - point->size) * 8);
		value = ft_reverse_bytes(value);
		ft_memmove(&assembler->bytecode[point->opr_pos + point->insert_pos],
				&value, point->size);
		points = points->next;
	}
}
