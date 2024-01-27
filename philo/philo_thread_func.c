/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_thread_func.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 16:58:56 by hyungdki          #+#    #+#             */
/*   Updated: 2024/01/27 19:41:31 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void		*philo_act(t_philo *val, t_arg *arg);
static int		philo_eating(t_philo *val, t_arg *arg);
static int		philo_sleep_think(t_philo *val, t_arg *arg);
static int		put_forks(t_fork *fork1, t_fork *fork2);

void	*philo_thread_func(void *param)
{
	t_philo	*val;
	t_arg	*arg;

	val = (t_philo *)param;
	arg = (t_arg *)val->arg;
	val->eat_cnt = 0;
	pthread_mutex_lock(&arg->start_flag);
	pthread_mutex_unlock(&arg->start_flag);
	if (chk_end(arg) != NORMAL)
		return (T_NULL);
	if (val->idx % 2 == 0 && usleep(arg->philo_num * 5) != 0)
		return (thread_error_end(arg));
	return (philo_act(val, arg));
}

inline static void	*philo_act(t_philo *val, t_arg *arg)
{
	int		func_result;

	while (chk_end(arg) == NORMAL)
	{
		func_result = philo_eating(val, arg);
		if (func_result == 1)
			return (thread_error_end(arg));
		else if (func_result == 2)
			return (T_NULL);
		if (philo_sleep_think(val, arg) == 1)
			return (thread_error_end(arg));
	}
	return (T_NULL);
}

inline static int	philo_eating(t_philo *val, t_arg *arg)
{	
	pthread_mutex_lock(&val->fork[0]->mtx);
	val->fork[0]->status = USING;
	if (chk_end(arg) != NORMAL || report(val, GET_FORK, arg) == FALSE)
		return (put_forks(val->fork[0], T_NULL));
	pthread_mutex_lock(&val->fork[1]->mtx);
	val->fork[1]->status = USING;
	if (chk_end(arg) != NORMAL || report(val, GET_FORK, arg) == FALSE
		|| report(val, EATING, arg) == FALSE)
		return (put_forks(val->fork[0], val->fork[1]));
	if (ft_usleep(arg->e_time * MS_TO_US, arg) == FALSE)
		return (put_forks(val->fork[0], val->fork[1]));
	put_forks(val->fork[0], val->fork[1]);
	if (arg->have_to_eat != -1 && ++(val->eat_cnt) == arg->have_to_eat)
	{
		pthread_mutex_lock(&arg->last_eat_mtx[val->idx]);
		val->end = TRUE;
		pthread_mutex_unlock(&arg->last_eat_mtx[val->idx]);
		if (report(val, EAT_DONE, arg) == FALSE)
			return (1);
		return (2);
	}
	return (0);
}

inline static int	philo_sleep_think(t_philo *val, t_arg *arg)
{
	if (chk_end(arg) != NORMAL || report(val, SLEEPING, arg) == FALSE)
		return (1);
	if (ft_usleep(arg->s_time * MS_TO_US, arg) == FALSE)
		return (1);
	if (chk_end(arg) != NORMAL || report(val, THINKING, arg) == FALSE)
		return (1);
	if (arg->philo_num % 2 == 1)
		usleep(arg->philo_num * 5);
	return (0);
}

static int	put_forks(t_fork *fork1, t_fork *fork2)
{
	if (fork1 != T_NULL)
	{
		fork1->status = UNUSE;
		pthread_mutex_unlock(&fork1->mtx);
	}
	if (fork2 != T_NULL)
	{
		fork2->status = UNUSE;
		pthread_mutex_unlock(&fork2->mtx);
	}
	return (1);
}
