/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_thread_func.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 16:58:56 by hyungdki          #+#    #+#             */
/*   Updated: 2023/08/19 18:55:59 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void *time_thread_func(void *input_arg)
{
	t_timeval time_lapse;
	t_arg *arg;

	arg = (t_arg *)input_arg;
	if (pthread_mutex_lock(&arg->start_flag) != 0)
		return (T_NULL);
	if (pthread_mutex_unlock(&arg->start_flag) != 0)
		return (T_NULL);
	if (gettimeofday(&time_lapse, T_NULL) != 0)
		return (exit_thread(arg, ABORT, T_NULL));
	
	return (T_NULL);
}
