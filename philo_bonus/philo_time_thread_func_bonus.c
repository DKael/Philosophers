/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_time_thread_func_bonus.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 16:58:56 by hyungdki          #+#    #+#             */
/*   Updated: 2023/08/27 18:35:31 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

void	*time_thread_func(void *input_arg)
{
	t_arg		*arg;
	t_philo		*data;
	long		time_lapse_usec;

	arg = ((t_arg_and_philo *)input_arg)->arg;
	data = ((t_arg_and_philo *)input_arg)->philo;
	sem_wait_nointr(arg->start_flag.sem);
	if (sem_post(arg->start_flag.sem) != 0)
		philo_process_end(arg);
	while (1)
	{
		time_lapse_usec = time_calc_from_start(arg);
		if (time_lapse_usec == -1)
			philo_process_end(arg);
		if (time_lapse_usec > data->last_eat + arg->d_time * MS_TO_US)
			philo_report(arg, data, DIE);
	}
	return (T_NULL);
}
