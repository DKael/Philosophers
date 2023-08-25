/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resource_free_func_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 19:33:08 by hyungdki          #+#    #+#             */
/*   Updated: 2023/08/25 13:37:38 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

int	arg_free(t_arg *arg)
{
	free(arg->philo);
	free(arg->last_eat_sem);
	free(arg->log_sem);
	return (1);
}

void	log_delete_func(void *log)
{
	t_log	*temp;

	if (log != T_NULL)
	{
		temp = (t_log *)log;
		free(temp);
	}
}

void	philos_log_clear(t_arg *arg, int cnt)
{
	int	idx;

	idx = -1;
	while (++idx < cnt)
		dll_clear(&arg->philo[idx].logs, log_delete_func);
}

void	arg_waitpids(t_arg *arg, int cnt)
{
	int	idx;

	idx = -1;
	while (++idx < cnt)
		waitpid(arg->philo[idx].philo_pid, T_NULL, 0);
}

void	arg_sems_destroy(t_arg *arg)
{
	int	idx;

	if (arg->start_flag_chk == TRUE)
		ft_sem_destroy(&arg->start_flag);
	if (arg->end_flag_sem_chk == TRUE)
		ft_sem_destroy(&arg->end_flag_sem);
	if (arg->fork_chk == TRUE)
		ft_sem_destroy(&arg->fork);
	idx = -1;
	while (++idx < arg->last_eat_sem_cnt)
	{
		free(arg->last_eat_sem[idx].name);
		ft_sem_destroy(&arg->last_eat_sem[idx]);
	}
	idx = -1;
	while (++idx < arg->log_sem_cnt)
	{
		free(arg->log_sem[idx].name);
		ft_sem_destroy(&arg->log_sem[idx]);
	}
}
