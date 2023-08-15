/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_thread_func.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 16:58:56 by hyungdki          #+#    #+#             */
/*   Updated: 2023/08/16 00:50:18 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void *philo_thread_func2(t_philo *value, t_arg *arg);
static void *philo_thread_func3(t_philo *value, t_arg *arg);

int ft_usleep(int ms)
{
	t_timeval start;
	t_timeval time_lapse;
	long us;
	long convert_ms_to_us;

	convert_ms_to_us = ms * 1000;
	gettimeofday(&start, T_NULL);
	gettimeofday(&time_lapse, T_NULL);
	us = (time_lapse.tv_sec - start.tv_sec) * 1000000 + (time_lapse.tv_usec - start.tv_usec);
	while (us < convert_ms_to_us)
	{
		usleep(10);
		gettimeofday(&time_lapse, T_NULL);
		us = (time_lapse.tv_sec - start.tv_sec) * 1000000 + (time_lapse.tv_usec - start.tv_usec);
	}

	return (0);
}

t_bool report(t_philo *value, t_philo_status status, t_timeval start)
{
	t_log *log;
	t_dllnode *log_node;

	log = (t_log *)malloc(sizeof(t_log));
	if (log == T_NULL)
		return (FALSE);
	if (gettimeofday(&log->time, T_NULL) != 0)
	{
		free(log);
		return (FALSE);
	}
	log->usec = (log->time.tv_sec - start.tv_sec) * 1000000 + (log->time.tv_usec - start.tv_usec);
	log->who = value->idx + 1;
	log->status = status;
	log_node = dll_new_node(log);
	if (log_node == T_NULL)
	{
		free(log);
		return (FALSE);
	}
	dll_add_tail(&value->logs, log_node);

	return (TRUE);
}

void *philo_thread_func(void *param)
{
	t_philo *value;
	t_arg *arg;

	value = (t_philo *)param;
	arg = (t_arg *)value->arg;
	value->eat_cnt = 0;
	while (arg->start_flag == FALSE)
	{
		if (usleep(10) != 0)
			printf("usleep function is interrupted by a signal\n");
		if (arg->da_flag != 0)
			return (T_NULL);
	}
	if (value->idx % 2 == 0)
		usleep(10);
	while (1)
	{
		if (philo_thread_func2(value, arg) == T_NULL)
			return (T_NULL);
	}
	return (T_NULL);
}

static void *philo_thread_func2(t_philo *value, t_arg *arg)
{

	if (arg->da_flag != 0 || pthread_mutex_lock(value->first_fork) != 0)
		return (exit_thread(arg, ABORT, &value->logs));
	if (arg->da_flag != 0 || pthread_mutex_lock(value->second_fork) != 0)
		return (exit_thread(arg, ABORT, &value->logs));
	if (arg->da_flag != 0 || report(value, GET_FORK, arg->start) == FALSE)
		return (exit_thread(arg, ABORT, &value->logs));
	if (arg->da_flag != 0 || report(value, EATING, arg->start) == FALSE)
		return (exit_thread(arg, ABORT, &value->logs));
	ft_usleep(arg->e_time);
	// while ( < arg->e_time * )
	// 	if (usleep(1000) != 0)
	// 		return (exit_thread(arg, ABORT, &value->logs));
	if (arg->da_flag != 0 || pthread_mutex_unlock(value->first_fork) != 0)
		return (exit_thread(arg, ABORT, &value->logs));
	if (arg->da_flag != 0 || pthread_mutex_unlock(value->second_fork) != 0)
		return (exit_thread(arg, ABORT, &value->logs));
	if (arg->da_flag != 0 || gettimeofday(&value->last_eat, T_NULL) != 0)
		return (exit_thread(arg, ABORT, &value->logs));
	return (philo_thread_func3(value, arg));
}

static void *philo_thread_func3(t_philo *value, t_arg *arg)
{
	if (arg->have_to_eat != -1 && ++(value->eat_cnt) == arg->have_to_eat)
	{
		arg->end_flag++;
		return (T_NULL);
	}
	if (arg->da_flag != 0 || report(value, SLEEPING, arg->start) == FALSE)
		return (exit_thread(arg, ABORT, &value->logs));
	ft_usleep(arg->s_time);
	if (arg->da_flag != 0 || report(value, THINKING, arg->start) == FALSE)
		return (exit_thread(arg, ABORT, &value->logs));
	return ((void *)1);
}