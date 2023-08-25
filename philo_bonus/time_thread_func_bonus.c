/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_thread_func_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 16:58:56 by hyungdki          #+#    #+#             */
/*   Updated: 2023/08/25 13:35:13 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

static int	time_thread_func2(t_arg *arg,
				long time_lapse_usec, t_bool *time_thrd_end);

void	*time_thread_func(void *input_arg)
{
	t_arg		*arg;
	t_timeval	time_lapse;
	long		time_lapse_usec;
	t_bool		time_thrd_end;

	arg = (t_arg *)input_arg;
	sem_wait_nointr(arg->start_flag.sem);
	sem_post(arg->start_flag.sem);
	if (check_end_flag(arg) != NORMAL)
		return (T_NULL);
	time_thrd_end = FALSE;
	while (check_end_flag(arg) == NORMAL && time_thrd_end == FALSE)
	{
		if (gettimeofday(&time_lapse, T_NULL) != 0)
			return (thread_error_end(arg));
		time_lapse_usec = (time_lapse.tv_sec - arg->start.tv_sec)
			* S_TO_US + (time_lapse.tv_usec - arg->start.tv_usec);
		if (time_thread_func2(arg, time_lapse_usec, &time_thrd_end) == 1)
			return (thread_error_end(arg));
	}
	return (T_NULL);
}

static int	time_thread_func2(t_arg *arg,
				long time_lapse_usec, t_bool *time_thrd_end)
{
	int		idx;

	idx = -1;
	while (++idx < arg->philo_num)
	{
		sem_wait_nointr(arg->last_eat_sem[idx].sem);
		if (arg->philo[idx].end == TRUE)
		{
			sem_post(arg->last_eat_sem[idx].sem);
			continue ;
		}
		if (time_lapse_usec > arg->philo[idx].last_eat + arg->d_time * MS_TO_US)
		{
			*time_thrd_end = TRUE;
			if (die_report(arg, time_lapse_usec, idx) == FALSE)
			{
				sem_post(arg->last_eat_sem[idx].sem);
				return (1);
			}
		}
		sem_post(arg->last_eat_sem[idx].sem);
	}
	return (0);
}
