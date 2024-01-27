/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_thread_func.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 16:58:56 by hyungdki          #+#    #+#             */
/*   Updated: 2024/01/27 17:05:03 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	time_thread_func2(t_arg *arg,
				long time_lapse_usec, t_bool *time_thrd_end);

void	*time_thread_func(void *input_arg)
{
	t_arg		*arg;
	t_timeval	time_lapse;
	long		time_lapse_usec;
	t_bool		time_thrd_end;

	arg = (t_arg *)input_arg;
	pthread_mutex_lock(&arg->start_flag);
	pthread_mutex_unlock(&arg->start_flag);
	if (chk_end(arg) != NORMAL)
		return (T_NULL);
	time_thrd_end = FALSE;
	while (chk_end(arg) == NORMAL && time_thrd_end == FALSE)
	{
		if (gettimeofday(&time_lapse, T_NULL) != 0)
			return (thread_error_end(arg));
		time_lapse_usec = time_calc(time_lapse, arg->start);
		if (time_thread_func2(arg, time_lapse_usec, &time_thrd_end) == 1)
			return (thread_error_end(arg));
	}
	return (T_NULL);
}

inline static int	time_thread_func2(t_arg *arg,
				long time_lapse_usec, t_bool *time_thrd_end)
{
	int		idx;

	idx = -1;
	while (++idx < arg->philo_num)
	{
		pthread_mutex_lock(&arg->last_eat_mtx[idx]);
		if (arg->philo[idx].end == TRUE)
		{
			pthread_mutex_unlock(&arg->last_eat_mtx[idx]);
			continue ;
		}
		if (time_lapse_usec > arg->philo[idx].last_eat + arg->d_time * MS_TO_US)
		{
			*time_thrd_end = TRUE;
			if (report(&arg->philo[idx], DIE, arg) == FALSE)
			{
				pthread_mutex_unlock(&arg->last_eat_mtx[idx]);
				return (1);
			}
		}
		pthread_mutex_unlock(&arg->last_eat_mtx[idx]);
	}
	return (0);
}
