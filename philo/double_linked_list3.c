/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   double_linked_list3.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 20:15:28 by hyungdki          #+#    #+#             */
/*   Updated: 2023/08/22 17:04:06 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "double_linked_list.h"

void	dll_del_node(t_dll *dll, t_dllnode *node, void (*del)(void *))
{
	if (dll_is_in(dll, node) == TRUE)
	{
		if (dll->size == 1)
			dll_init(dll);
		else if (node->front == &(dll->head))
		{
			dll->head.back = node->back;
			node->back->front = &(dll->head);
		}
		else if (node->front == &(dll->head))
		{
			dll->tail.front = node->front;
			node->front->back = &(dll->tail);
		}
		else
		{
			node->front->back = node->back;
			node->back->front = node->front;
		}
		del(node->contents);
		free(node);
		dll->size--;
	}
}

int	dll_node_compare(t_dllnode *n1, t_dllnode *n2,
	int (*compare)(t_dllnode *, t_dllnode *))
{
	int	result;

	result = compare(n1, n2);
	if (result == 1)
		return (1);
	else if (result == -1)
		return (-1);
	else
		return (0);
}

t_bool	dll_node_move_to_another_dll_head(t_dllnode *n,
				t_dll *dll1, t_dll *dll2)
{
	if (n->front != T_NULL && n->back != T_NULL)
	{
		n->back->front = n->front;
		n->front->back = n->back;
		dll1->size--;
		dll_add_head(dll2, n);
		return (TRUE);
	}
	return (FALSE);
}

t_bool	dll_node_move_to_another_dll_tail(t_dllnode *n,
				t_dll *dll1, t_dll *dll2)
{
	if (n->front != T_NULL && n->back != T_NULL)
	{
		n->back->front = n->front;
		n->front->back = n->back;
		dll1->size--;
		dll_add_tail(dll2, n);
		return (TRUE);
	}
	return (FALSE);
}
