/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_thread_func.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 16:58:56 by hyungdki          #+#    #+#             */
/*   Updated: 2023/08/21 19:27:27 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

t_bool die_report(t_arg *arg, long time_lapse_usec, int idx)
{
	t_log *log;
	t_dllnode *log_node;

	log = (t_log *)malloc(sizeof(t_log));
	if (log == T_NULL)
		return (FALSE);
	log->usec = time_lapse_usec;
	log->who = idx + 1;
	log->status = DIE;
	log_node = dll_new_node(log);
	if (log_node == T_NULL)
	{
		free(log);
		return (FALSE);
	}
	pthread_mutex_lock(&arg->log_mtx[idx]);
	dll_add_tail(&arg->philo[idx].logs, log_node);
	pthread_mutex_unlock(&arg->log_mtx[idx]);
	return (TRUE);
}

void *time_thread_func(void *input_arg)
{
	t_arg *arg;

	arg = (t_arg *)input_arg;
	pthread_mutex_lock(&arg->start_flag);
	pthread_mutex_unlock(&arg->start_flag);
	if (check_end_flag(arg) != 0)
		return (T_NULL);

	int idx;
	t_timeval time_lapse;
	long time_lapse_usec;
	t_bool time_thrd_end;
	long	revision;

	time_thrd_end = FALSE;
	while (check_end_flag(arg) == 0)
	{
		if (gettimeofday(&time_lapse, T_NULL) != 0)
			thread_error_end(arg);
		time_lapse_usec = (time_lapse.tv_sec - arg->start.tv_sec) * S_TO_US + (time_lapse.tv_usec - arg->start.tv_usec);
		idx = -1;
		while (++idx < arg->philo_num)
		{
			pthread_mutex_lock(&arg->last_eat_mtx[idx]);
			if  (arg->philo[idx].last_eat == -1)
			{
				pthread_mutex_unlock(&arg->last_eat_mtx[idx]);
				continue;
			}
			revision = time_lapse_usec - arg->philo[idx].last_eat - arg->d_time * MS_TO_US;
			if (revision > 0)
			{
				time_thrd_end = TRUE;
				if (die_report(arg, time_lapse_usec - revision, idx) == FALSE)
				{
					pthread_mutex_unlock(&arg->last_eat_mtx[idx]);
					return (thread_error_end(arg));
				}
			}
			pthread_mutex_unlock(&arg->last_eat_mtx[idx]);
		}
		if (time_thrd_end == TRUE)
			break;
	}
	return (T_NULL);
}
