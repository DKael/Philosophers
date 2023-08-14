/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_thread_func.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 16:58:56 by hyungdki          #+#    #+#             */
/*   Updated: 2023/08/14 18:58:57 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	*philo_thread_func2(t_philo *value, t_arg *arg);
static void	*philo_thread_func3(t_philo *value, t_arg *arg);

void	*philo_thread_func(void *param)
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
	while (1)
	{
		if (philo_thread_func2(value, arg) == T_NULL)
			return (T_NULL);
	}
	return (T_NULL);
}

static void	*philo_thread_func2(t_philo *value, t_arg *arg)
{
	int cnt;

	if (arg->da_flag != 0 || pthread_mutex_lock(value->first_fork) != 0)
		return (exit_thread(arg, ABORT, &value->logs));
	if (arg->da_flag != 0 || pthread_mutex_lock(value->second_fork) != 0)
		return (exit_thread(arg, ABORT, &value->logs));
	if (arg->da_flag != 0 || report(value, GET_FORK) == FALSE)
		return (exit_thread(arg, ABORT, &value->logs));
	if (arg->da_flag != 0 || report(value, EATING) == FALSE)
		return (exit_thread(arg, ABORT, &value->logs));
	cnt = -1;
	while (++cnt < arg->e_time)
		if (arg->da_flag != 0 || usleep(1000) != 0)
			return (exit_thread(arg, ABORT, &value->logs));
	if (arg->da_flag != 0 || pthread_mutex_unlock(value->first_fork) != 0)
		return (exit_thread(arg, ABORT, &value->logs));
	if (arg->da_flag != 0 || pthread_mutex_unlock(value->second_fork) != 0)
		return (exit_thread(arg, ABORT, &value->logs));
	if (arg->da_flag != 0 || gettimeofday(&value->last_eat, T_NULL) != 0)
		return (exit_thread(arg, ABORT, &value->logs));
	return (philo_thread_func3(value, arg));
}

static void	*philo_thread_func3(t_philo *value, t_arg *arg)
{
	int cnt;
	
	if (arg->have_to_eat != -1 && ++value->eat_cnt == arg->have_to_eat)
	{
		arg->end_flag++;
		return (T_NULL);
	}
	if (arg->da_flag != 0 || report(value, SLEEPING) == FALSE)
		return (exit_thread(arg, ABORT, &value->logs));
	cnt = -1;
	while (++cnt < arg->s_time)
		if (arg->da_flag != 0 || usleep(1000) != 0)
			return (exit_thread(arg, ABORT, &value->logs));
	if (arg->da_flag != 0 || report(value, THINKING) == FALSE)
		return (exit_thread(arg, ABORT, &value->logs));
	return ((void *)1);
}