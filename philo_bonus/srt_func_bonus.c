/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   srt_func_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/22 16:37:00 by hyungdki          #+#    #+#             */
/*   Updated: 2023/08/23 09:54:08 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

int	srt_time_compare(void *input_lst, int idx1, int idx2)
{
	t_srt	*lst;

	lst = (t_srt *)input_lst;
	if (lst[idx1].usec > lst[idx2].usec)
		return (1);
	else if (lst[idx1].usec < lst[idx2].usec)
		return (-1);
	else
	{
		if (lst[idx1].status > lst[idx2].status)
			return (1);
		else if (lst[idx1].status < lst[idx2].status)
			return (-1);
		else
			return (0);
	}
}

void	srt_swap(void *input_lst, int idx1, int idx2)
{
	t_srt	*lst;
	t_srt	temp;

	lst = (t_srt *)input_lst;
	temp = lst[idx1];
	lst[idx1] = lst[idx2];
	lst[idx2] = temp;
}
