/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_process_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 16:58:56 by hyungdki          #+#    #+#             */
/*   Updated: 2023/08/27 18:45:02 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

static void	philo_thread_func2(t_arg *arg, t_philo *data);
static void	philo_init(t_philo *data, int idx);

void	philo_process_func(t_arg *arg, int idx)
{
	t_philo			data;
	t_arg_and_philo	to_time_func;

	philo_init(&data, idx);
	to_time_func.arg = arg;
	to_time_func.philo = &data;
	if (pthread_create(&data.time_thrd, T_NULL,
			time_thread_func, &to_time_func) != 0)
		exit (1);
	sem_wait_nointr(arg->start_flag.sem);
	if (sem_post(arg->start_flag.sem) != 0)
		philo_process_end(arg);
	if (data.idx % 2 == 0 && usleep(arg->philo_num * 10) != 0)
		philo_process_end(arg);
	philo_thread_func2(arg, &data);
}

static void	philo_thread_func2(t_arg *arg, t_philo *data)
{
	while (1)
	{
		sem_wait_nointr(arg->fork.sem);
		sem_wait_nointr(arg->fork.sem);
		philo_report(arg, data, GET_FORK);
		philo_report(arg, data, EATING);
		if (ft_usleep(arg->e_time * MS_TO_US) == FALSE)
			philo_process_end(arg);
		if (sem_post(arg->fork.sem) != 0 || sem_post(arg->fork.sem) != 0)
			philo_process_end(arg);
		if (arg->have_to_eat != -1 && ++(data->eat_cnt) == arg->have_to_eat)
			break ;
		philo_report(arg, data, SLEEPING);
		if (ft_usleep(arg->s_time * MS_TO_US) == FALSE)
			philo_process_end(arg);
		philo_report(arg, data, THINKING);
		if (arg->philo_num % 2 == 1)
			usleep(arg->philo_num * 4);
	}
	exit(0);
}

static void	philo_init(t_philo *data, int idx)
{
	data->idx = idx;
	data->last_eat = 0;
	data->eat_cnt = 0;
}

void	philo_process_end(t_arg *arg)
{
	sem_wait_nointr(arg->print_sem.sem);
	exit(1);
}

void	philo_report(t_arg *arg, t_philo *data, t_philo_status status)
{
	long	time;

	time = time_calc_from_start(arg);
	if (time == -1)
		philo_process_end(arg);
	sem_wait_nointr(arg->print_sem.sem);
	if (status == GET_FORK)
		printf("%ld %d has taken a fork\n", time / 1000, data->idx + 1);
	else if (status == EATING)
	{
		data->last_eat = time;
		printf("%ld %d is eating\n", time / 1000, data->idx + 1);
	}
	else if (status == SLEEPING)
		printf("%ld %d is sleeping\n", time / 1000, data->idx + 1);
	else if (status == THINKING)
		printf("%ld %d is thinking\n", time / 1000, data->idx + 1);
	else if (status == DIE)
	{
		printf("%ld %d died\n", time / 1000, data->idx + 1);
		exit(1);
	}
	if (sem_post(arg->print_sem.sem) != 0)
		philo_process_end(arg);
}
