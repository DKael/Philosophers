/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quick_sort_bonus.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 15:01:01 by hyungdki          #+#    #+#             */
/*   Updated: 2023/08/23 09:53:48 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef QUICK_SORT_BONUS_H
# define QUICK_SORT_BONUS_H

typedef struct s_data
{
	void	*part;
	int		left;
	int		right;
	int		(*compare_func)(void *, int, int);
	void	(*swap_func)(void *, int, int);
}	t_data;

void	sorting(void *lst, int size,
			int (*compare_func)(void *, int, int),
			void (*swap_func)(void *, int, int));
#endif