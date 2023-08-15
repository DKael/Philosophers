/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quick_sort.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 15:01:01 by hyungdki          #+#    #+#             */
/*   Updated: 2023/08/15 22:28:18 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef QUICK_SORT_H
# define QUICK_SORT_H

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