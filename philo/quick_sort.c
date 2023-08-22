/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quick_sort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 15:01:17 by hyungdki          #+#    #+#             */
/*   Updated: 2023/08/21 19:27:29 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quick_sort.h"

static void	quick_sort(t_data data);
static int	make_partition(const t_data *data);

void	sorting(void *lst, int size,
	int (*compare_func)(void *, int, int),
	void (*swap_func)(void *, int, int))
{
	t_data	data;

	data.part = lst;
	data.left = 0;
	data.right = size - 1;
	data.compare_func = compare_func;
	data.swap_func = swap_func;
	quick_sort(data);
}

void	quick_sort(t_data data)
{
	int		pivot;
	t_data	left_data;
	t_data	right_data;

	if (data.left < data.right)
	{
		pivot = make_partition(&data);
		left_data = data;
		left_data.right = pivot - 1;
		quick_sort(left_data);
		right_data = data;
		right_data.left = pivot + 1;
		quick_sort(right_data);
	}
}

int	make_partition(const t_data *data)
{
	int	pivot;
	int	head;
	int	tail;

	pivot = data->left;
	head = data->left;
	tail = data->right + 1;
	while (head < tail)
	{
		while (++head < data->right
			&& data->compare_func(data->part, head, pivot) == -1)
			;
		while (--tail > data->left
			&& data->compare_func(data->part, tail, pivot) == 1)
			;
		if (head < tail)
			data->swap_func(data->part, head, tail);
	}
	data->swap_func(data->part, pivot, tail);
	return (tail);
}
