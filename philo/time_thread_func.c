/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_thread_func.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 16:58:56 by hyungdki          #+#    #+#             */
/*   Updated: 2023/08/14 19:01:09 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void *time_thread_func(void *input_arg)
{
	t_timeval time_lapse;
	t_arg *arg;

	arg = (t_arg *)input_arg;
	while (arg->start_flag == FALSE)
	{
		if (usleep(10) != 0)
			printf("usleep function is interrupted by a signal\n");
		if (arg->da_flag != 0)
			return (T_NULL);
	}
	if (gettimeofday(&time_lapse, T_NULL) != 0)
		return (exit_thread(arg, ABORT, T_NULL));
	return (T_NULL);
}
