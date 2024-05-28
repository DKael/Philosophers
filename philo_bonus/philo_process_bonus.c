/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_process_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 16:58:56 by hyungdki          #+#    #+#             */
/*   Updated: 2023/08/27 20:13:28 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

static void		philo_thread_func2(t_arg *arg, t_philo *data);
static int		philo_thread_func3(t_arg *arg, t_philo *data);
static t_bool	death_chk(t_arg *arg, t_philo *data);
static void		philo_report(t_arg *arg, t_philo *data, t_philo_status status);

void	philo_process_func(t_arg *arg, int idx)
{
	t_philo			data;

	data.idx = idx;
	data.eat_cnt = 0;
	sem_wait_nointr(arg->start_flag.sem);
	if (sem_post(arg->start_flag.sem) != 0)
	{
		sem_wait_nointr(arg->print_sem.sem);
		exit(1);
	}
	gettimeofday(&data.last_eat, T_NULL);
	if (arg->philo_num == 1)
	{
		philo_report(arg, &data, GET_FORK);
		while (death_chk(arg, &data) == FALSE)
			;
		philo_report(arg, &data, DIE);
	}
	if (data.idx % 2 == 0)
		usleep(2000);
	philo_thread_func2(arg, &data);
	exit(0);
}

static void	philo_thread_func2(t_arg *arg, t_philo *data)
{
	while (1)
	{
		sem_wait_nointr(arg->fork.sem);
		if (death_chk(arg, data) == TRUE)
			philo_report(arg, data, DIE);
		else
			philo_report(arg, data, GET_FORK);
		sem_wait_nointr(arg->fork.sem);
		if (death_chk(arg, data) == TRUE)
			philo_report(arg, data, DIE);
		else
			philo_report(arg, data, GET_FORK);
		if (death_chk(arg, data) == TRUE)
			philo_report(arg, data, DIE);
		else
			philo_report(arg, data, EATING);
		if (philo_thread_func3(arg, data) == 1)
			break ;
	}
	exit(0);
}

static int	philo_thread_func3(t_arg *arg, t_philo *data)
{
	if (ft_usleep(arg->e_time * MS_TO_US, data->last_eat, arg->d_time) == FALSE)
		philo_report(arg, data, DIE);
	if (sem_post(arg->fork.sem) != 0 || sem_post(arg->fork.sem) != 0)
	{
		sem_wait_nointr(arg->print_sem.sem);
		exit(1);
	}
	if (arg->have_to_eat != -1 && ++(data->eat_cnt) == arg->have_to_eat)
		return (1);
	if (death_chk(arg, data) == TRUE)
		philo_report(arg, data, DIE);
	else
		philo_report(arg, data, SLEEPING);
	if (ft_usleep(arg->s_time * MS_TO_US, data->last_eat, arg->d_time) == FALSE)
		philo_report(arg, data, DIE);
	if (death_chk(arg, data) == TRUE)
		philo_report(arg, data, DIE);
	else
		philo_report(arg, data, THINKING);
	return (0);
}

static t_bool	death_chk(t_arg *arg, t_philo *data)
{
	t_timeval	chk_time;

	gettimeofday(&chk_time, T_NULL);
	if (time_calc(chk_time, data->last_eat) > arg->d_time * MS_TO_US)
		return (TRUE);
	return (FALSE);
}

static void	philo_report(t_arg *arg, t_philo *data, t_philo_status status)
{
	t_timeval	time;
	char		buf[BUF_SIZE];
	int			len;

	gettimeofday(&time, T_NULL);
	if (status == EATING)
		data->last_eat = time;
	len = ft_itoa(time_calc(time, arg->start) / 1000, buf);
	buf[len++] = ' ';
	len += ft_itoa(data->idx + 1, &buf[len]);
	if (status == GET_FORK)
		ft_strcat(&buf[len], " has taken a fork\n");
	else if (status == EATING)
		ft_strcat(&buf[len], " is eating\n");
	else if (status == SLEEPING)
		ft_strcat(&buf[len], " is sleeping\n");
	else if (status == THINKING)
		ft_strcat(&buf[len], " is thinking\n");
	else if (status == DIE)
		ft_strcat(&buf[len], " died\n");
	sem_wait_nointr(arg->print_sem.sem);
	write(1, buf, ft_strlen(buf));
	if (status == DIE || sem_post(arg->print_sem.sem) != 0)
		exit(1);
}
