/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_time_thread_func_bonus.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 16:58:56 by hyungdki          #+#    #+#             */
/*   Updated: 2023/08/27 20:12:04 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

void	*time_thread_func(void *input_arg)
{
	t_arg		*arg;
	t_philo		*data;
	long		time;

	arg = ((t_arg_and_philo *)input_arg)->arg;
	data = ((t_arg_and_philo *)input_arg)->philo;
	sem_wait_nointr(arg->start_flag.sem);
	if (sem_post(arg->start_flag.sem) != 0)
		philo_process_end(arg);
	while (1)
	{
		time = time_calc_from_start(arg);
		if (time == -1)
			philo_process_end(arg);
		if (time > data->last_eat + arg->d_time * MS_TO_US)
		{
			sem_wait_nointr(arg->print_sem.sem);
			time = time_calc_from_start(arg);
			if (time == -1)
				exit(1);
			printf("%ld %d died\n", time / 1000, data->idx + 1);
			exit(1);
		}
	}
	return (T_NULL);
}
