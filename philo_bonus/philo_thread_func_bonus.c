/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_thread_func_bonus.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 16:58:56 by hyungdki          #+#    #+#             */
/*   Updated: 2023/08/26 21:32:43 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

static void		*philo_thread_func2_1(t_philo *value, t_arg *arg);
static int		philo_thread_func2_2(t_philo *value, t_arg *arg);
static int		philo_thread_func2_3(t_philo *value, t_arg *arg);

void	philo_init(t_philo *data, int idx)
{
	data->idx = idx;
	data->last_eat = 0;
	data->last_eat_sem.name = T_NULL;
	data->last_eat_sem_chk = FALSE;
	data->end_flag = NORMAL;
	data->end_flag_sem.name = T_NULL;
	data->end_flag_sem_chk = FALSE;
	data->end = FALSE;
	data->eat_cnt = 0;
	data->time_thrd_chk = FALSE;
}

void	philo_process_err(t_philo *data)
{
	free(data->last_eat_sem.name);
	if (data->last_eat_sem_chk == TRUE)
		ft_sem_destroy(&data->last_eat_sem);
	free(data->end_flag_sem.name);
	if (data->end_flag_sem_chk == TRUE)
		ft_sem_destroy(&data->end_flag_sem);
	if (data->time_thrd_chk = TRUE)
		pthread_join(data->time_thrd, T_NULL);
	exit(1);
}

void	philo_process_func(t_arg *arg, int idx)
{
	t_philo	data;
	char	*temp;

	philo_init(&data, idx);
	temp = ft_itoa(idx);
	if (temp == T_NULL)
		return (1);
	data.last_eat_sem.name = ft_strjoin("le_sem", temp);
	data.end_flag_sem.name = ft_strjoin("e_flag", temp);
	free(temp);
	if (data.last_eat_sem.name == T_NULL || data.end_flag_sem.name == T_NULL)
		return (philo_process_err(&data));
	data.last_eat_sem.sem = ft_sem_open(data.last_eat_sem.name, 0644, 1);
	if (data.last_eat_sem.sem == SEM_FAILED)
		return (philo_process_err(&data));
	data.last_eat_sem_chk = TRUE;
	data.end_flag_sem.sem = ft_sem_open(data.last_eat_sem.name, 0644, 1);
		if (data.end_flag_sem.sem == SEM_FAILED)
		return (philo_process_err(&data));
	data.end_flag_sem_chk = TRUE;
	if (pthread_create(&data.time_thrd, T_NULL, time_thread_func, &data) != 0)
		return (philo_process_err(&data));
	data.time_thrd_chk = TRUE;
	sem_wait_nointr(arg->start_flag.sem);
	sem_post(arg->start_flag.sem);
	
	if (data.idx % 2 == 0 && usleep(arg->philo_num * 10) != 0)
		return (philo_process_err(&data));
	return (philo_thread_func2_1(value, arg));
}

static void	*philo_thread_func2_1(t_philo *data, t_arg *arg)
{
	int		func_result;

	while (1)
	{
		sem_wait_nointr(arg->fork.sem);
		sem_wait_nointr(arg->fork.sem);
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
