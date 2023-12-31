/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_thread_func.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 16:58:56 by hyungdki          #+#    #+#             */
/*   Updated: 2023/11/30 18:47:05 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void		*philo_thread_func2_1(t_philo *value, t_arg *arg);
static int		philo_thread_func2_2(t_philo *value, t_arg *arg);
static int		philo_thread_func2_3(t_philo *value, t_arg *arg);

void	*philo_thread_func(void *param)
{
	t_philo	*value;
	t_arg	*arg;

	value = (t_philo *)param;
	arg = (t_arg *)value->arg;
	value->eat_cnt = 0;
	pthread_mutex_lock(&arg->start_flag);
	pthread_mutex_unlock(&arg->start_flag);
	if (check_end_flag(arg) != NORMAL)
		return (T_NULL);
	pthread_mutex_lock(&arg->last_eat_mtx[value->idx]);
	value->last_eat = 0;
	pthread_mutex_unlock(&arg->last_eat_mtx[value->idx]);
	if (arg->philo_num == 1)
	{
		while (check_end_flag(arg) == 0)
			usleep(1000);
		return (T_NULL);
	}
	if (value->idx % 2 == 0 && usleep(arg->philo_num * 10) != 0)
		return (thread_error_end(arg));
	return (philo_thread_func2_1(value, arg));
}

static void	*philo_thread_func2_1(t_philo *value, t_arg *arg)
{
	int		func_result;

	while (check_end_flag(arg) == NORMAL)
	{
		if (philo_thread_func2_2(value, arg) == 1)
			return (thread_error_end(arg));
		pthread_mutex_unlock(value->first_fork);
		pthread_mutex_unlock(value->second_fork);
		func_result = philo_thread_func2_3(value, arg);
		if (func_result == 0)
			return (T_NULL);
		else if (func_result == 1)
			return (thread_error_end(arg));
	}
	return (T_NULL);
}

static int	philo_thread_func2_2(t_philo *value, t_arg *arg)
{
	pthread_mutex_lock(value->first_fork);
	if (check_end_flag(arg) != NORMAL
		|| report(value, GET_FORK, arg) == FALSE)
	{
		pthread_mutex_unlock(value->first_fork);
		return (1);
	}
	pthread_mutex_lock(value->second_fork);
	if (check_end_flag(arg) != NORMAL
		|| report(value, GET_FORK, arg) == FALSE
		|| report(value, EATING, arg) == FALSE)
	{
		pthread_mutex_unlock(value->first_fork);
		pthread_mutex_unlock(value->second_fork);
		return (1);
	}
	if (check_end_flag(arg) != NORMAL
		|| ft_usleep(arg->e_time * MS_TO_US, arg) == FALSE)
	{
		pthread_mutex_unlock(value->first_fork);
		pthread_mutex_unlock(value->second_fork);
		return (1);
	}
	return (0);
}

static int	philo_thread_func2_3(t_philo *value, t_arg *arg)
{
	if (arg->have_to_eat != -1 && ++(value->eat_cnt) == arg->have_to_eat)
	{
		pthread_mutex_lock(&arg->last_eat_mtx[value->idx]);
		value->end = TRUE;
		pthread_mutex_unlock(&arg->last_eat_mtx[value->idx]);
		if (report(value, EAT_DONE, arg) == FALSE)
			return (1);
		return (0);
	}
	if (check_end_flag(arg) != NORMAL
		|| report(value, SLEEPING, arg) == FALSE)
		return (1);
	if (check_end_flag(arg) != NORMAL
		|| ft_usleep(arg->s_time * MS_TO_US, arg) == FALSE)
		return (1);
	if (check_end_flag(arg) != NORMAL
		|| report(value, THINKING, arg) == FALSE)
		return (1);
	if (arg->philo_num % 2 == 1)
		usleep(arg->philo_num * 5);
	return (2);
}
