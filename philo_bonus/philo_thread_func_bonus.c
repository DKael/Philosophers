/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_thread_func_bonus.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 16:58:56 by hyungdki          #+#    #+#             */
/*   Updated: 2023/08/25 19:04:46 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

static void		*philo_thread_func2_1(t_philo *value, t_arg *arg);
static int		philo_thread_func2_2(t_philo *value, t_arg *arg);
static int		philo_thread_func2_3(t_philo *value, t_arg *arg);

void	*philo_thread_func(t_arg *arg, t_philo *value)
{
	sem_wait_nointr(arg->start_flag.sem);
	sem_post(arg->start_flag.sem);
	if (check_end_flag(arg) != NORMAL)
		return (T_NULL);
	printf("%d test12, errno : %d\n", value->idx, errno);
	sem_wait_nointr(arg->last_eat_sem[value->idx].sem);
	value->last_eat = 0;
	sem_post(arg->last_eat_sem[value->idx].sem);
	printf("%d test13\n", value->idx);
	if (arg->philo_num == 1)
	{
		while (check_end_flag(arg) == 0)
			if (usleep(1000) == EINTR)
				printf("Interrupted by a signa\n");
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
		sem_wait_nointr(arg->fork.sem);
		printf("%d philo get first fork\n", value->idx);
		sem_wait_nointr(arg->fork.sem);
		printf("%d philo get seconde fork\n", value->idx);
		if (philo_thread_func2_2(value, arg) == 1)
			return (thread_error_end(arg));
		sem_post(arg->fork.sem);
		sem_post(arg->fork.sem);
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
	if (check_end_flag(arg) != NORMAL
		|| report(value, GET_FORK, arg) == FALSE
		|| report(value, EATING, arg) == FALSE)
	{
		sem_post(arg->fork.sem);
		sem_post(arg->fork.sem);
		return (1);
	}
	if (check_end_flag(arg) != NORMAL
		|| ft_usleep(arg->e_time * MS_TO_US) == FALSE)
	{
		sem_post(arg->fork.sem);
		sem_post(arg->fork.sem);
		return (1);
	}
	return (0);
}

static int	philo_thread_func2_3(t_philo *value, t_arg *arg)
{
	if (arg->have_to_eat != -1 && ++(value->eat_cnt) == arg->have_to_eat)
	{
		sem_wait_nointr(arg->last_eat_sem[value->idx].sem);
		value->end = TRUE;
		sem_post(arg->last_eat_sem[value->idx].sem);
		if (report(value, EAT_DONE, arg) == FALSE)
			return (1);
		return (0);
	}
	if (check_end_flag(arg) != NORMAL
		|| report(value, SLEEPING, arg) == FALSE)
		return (1);
	if (check_end_flag(arg) != NORMAL
		|| ft_usleep(arg->s_time * MS_TO_US) == FALSE)
		return (1);
	if (check_end_flag(arg) != NORMAL
		|| report(value, THINKING, arg) == FALSE)
		return (1);
	if (arg->philo_num % 2 == 1 && usleep(arg->philo_num * 5) == EINTR)
		printf("Interrupted by a signa\n");
	return (2);
}
