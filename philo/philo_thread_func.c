/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_thread_func.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 16:58:56 by hyungdki          #+#    #+#             */
/*   Updated: 2023/08/21 19:01:26 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void *philo_thread_func2(t_philo *value, t_arg *arg);

t_bool report(t_philo *value, t_philo_status status, t_arg *arg)
{
	t_log *log;
	t_dllnode *log_node;

	log = (t_log *)malloc(sizeof(t_log));
	if (log == NULL)
		return (FALSE);
	if (gettimeofday(&log->time, NULL) != 0)
	{
		free(log);
		return (FALSE);
	}
	log->usec = (log->time.tv_sec - arg->start.tv_sec) * S_TO_US + (log->time.tv_usec - arg->start.tv_usec);
	if (status == EATING)
	{
		pthread_mutex_lock(&arg->last_eat_mtx[value->idx]);
		value->last_eat = log->usec;
		pthread_mutex_unlock(&arg->last_eat_mtx[value->idx]);
	}
	log->who = value->idx + 1;
	log->status = status;
	log_node = dll_new_node(log);
	if (log_node == NULL)
	{
		free(log);
		return (FALSE);
	}
	pthread_mutex_lock(&arg->log_mtx[value->idx]);
	dll_add_tail(&value->logs, log_node);
	pthread_mutex_unlock(&arg->log_mtx[value->idx]);
	return (TRUE);
}

void *philo_thread_func(void *param)
{
	t_philo *value;
	t_arg *arg;

	value = (t_philo *)param;
	arg = (t_arg *)value->arg;
	value->eat_cnt = 0;
	pthread_mutex_lock(&arg->start_flag);
	pthread_mutex_unlock(&arg->start_flag);
	if (check_end_flag(arg) != 0)
		return (NULL);
	pthread_mutex_lock(&arg->last_eat_mtx[value->idx]);
	value->last_eat = 0;
	pthread_mutex_unlock(&arg->last_eat_mtx[value->idx]);
	if (arg->philo_num == 1)
	{
		while (check_end_flag(arg) == 0)
			if (usleep(1000) == EINTR)
				printf("Interrupted by a signa\n");
		return (NULL);
	}
	if (value->idx % 2 == 0 && usleep(arg->philo_num * 10) == EINTR)
		printf("Interrupted by a signal\n");
	return (philo_thread_func2(value, arg));
}

static void *philo_thread_func2(t_philo *value, t_arg *arg)
{
	while (check_end_flag(arg) == 0)
	{
		pthread_mutex_lock(value->first_fork);
		pthread_mutex_lock(value->second_fork);
		if (check_end_flag(arg) != 0 || report(value, GET_FORK, arg) == FALSE || report(value, EATING, arg) == FALSE)
		{
			
			pthread_mutex_unlock(value->first_fork);
			pthread_mutex_unlock(value->second_fork);
			return (thread_error_end(arg));
		}
		if (check_end_flag(arg) != 0 || ft_usleep(arg->e_time * MS_TO_US) == FALSE)
		{
			
			pthread_mutex_unlock(value->first_fork);
			pthread_mutex_unlock(value->second_fork);
			return (thread_error_end(arg));
		}
		
		pthread_mutex_unlock(value->first_fork);
		pthread_mutex_unlock(value->second_fork);
		if (arg->have_to_eat != -1 && ++(value->eat_cnt) == arg->have_to_eat)
		{
			pthread_mutex_lock(&arg->last_eat_mtx[value->idx]);
			value->last_eat = -1;
			pthread_mutex_unlock(&arg->last_eat_mtx[value->idx]);
			return (NULL);
		}

		if (check_end_flag(arg) != 0 || report(value, SLEEPING, arg) == FALSE)
			return (thread_error_end(arg));
		if (check_end_flag(arg) != 0 || ft_usleep(arg->s_time * MS_TO_US) == FALSE)
			return (thread_error_end(arg));
		if (check_end_flag(arg) != 0 || report(value, THINKING, arg) == FALSE)
			return (thread_error_end(arg));
		if (arg->philo_num % 2 == 1 && usleep(arg->philo_num * 5) == EINTR)
			printf("Interrupted by a signa\n");
	}
	return (NULL);
}