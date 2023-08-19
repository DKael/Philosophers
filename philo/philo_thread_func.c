/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_thread_func.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 16:58:56 by hyungdki          #+#    #+#             */
/*   Updated: 2023/08/19 17:44:36 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void *philo_thread_func2(t_philo *value, t_arg *arg);

t_bool ft_usleep(int ms)
{
	t_timeval start;
	t_timeval t;
	long us;
	long convert_ms_to_us;
	long sleep_time;

	convert_ms_to_us = ms * 1000;
	if (gettimeofday(&start, T_NULL) != 0 || gettimeofday(&t, T_NULL) != 0)
		return (FALSE);
	us = (t.tv_sec - start.tv_sec) * S_TO_US + (t.tv_usec - start.tv_usec);
	sleep_time = convert_ms_to_us;
	while (us < convert_ms_to_us)
	{
		if (sleep_time > 10)
			sleep_time /= 5;
		else
			sleep_time = 1;
		if (usleep(sleep_time) != 0)
			printf("usleep function is interrupted by a signal\n");
		if (gettimeofday(&t, T_NULL) != 0)
			return (FALSE);
		us = (t.tv_sec - start.tv_sec) * S_TO_US + (t.tv_usec - start.tv_usec);
	}
	return (TRUE);
}

t_bool report(t_philo *value, t_philo_status status, t_arg *arg)
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
	log->usec = (log->time.tv_sec - arg->start.tv_sec) * S_TO_US
		+ (log->time.tv_usec - arg->start.tv_usec);
	if (status == EATING)
		value->last_eat = log->usec;
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
	if (pthread_mutex_lock(&arg->start_flag) != 0)
		return (T_NULL);
	if (pthread_mutex_unlock(&arg->start_flag) != 0)
		return (T_NULL);
	if (value->idx % 2 == 0)
		usleep(arg->philo_num);
	return (philo_thread_func2(value, arg));
}

static void *philo_thread_func2(t_philo *value, t_arg *arg)
{
	while (1)
	{
		if (arg->da_flag != 0 || pthread_mutex_lock(value->first_fork) != 0 || pthread_mutex_lock(value->second_fork) != 0)
			return (exit_thread(arg, ABORT, &value->logs));
		if (arg->da_flag != 0 || report(value, GET_FORK, arg) == FALSE || report(value, EATING, arg) == FALSE)
			return (exit_thread(arg, ABORT, &value->logs));
		if (arg->da_flag != 0 || ft_usleep(arg->e_time) == FALSE)
			return (exit_thread(arg, ABORT, &value->logs));
		if (arg->da_flag != 0 || pthread_mutex_unlock(value->first_fork) != 0 || pthread_mutex_unlock(value->second_fork) != 0)
			return (exit_thread(arg, ABORT, &value->logs));
		if (arg->have_to_eat != -1 && ++(value->eat_cnt) == arg->have_to_eat)
			return (exit_thread(arg, END, &value->logs));
		if (arg->da_flag != 0 || report(value, SLEEPING, arg) == FALSE)
			return (exit_thread(arg, ABORT, &value->logs));
		if (arg->da_flag != 0 || ft_usleep(arg->s_time) == FALSE)
			return (exit_thread(arg, ABORT, &value->logs));
		if (arg->da_flag != 0 || report(value, THINKING, arg) == FALSE)
			return (exit_thread(arg, ABORT, &value->logs));
	}
	return (T_NULL);
}