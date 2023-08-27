/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resource_free_func_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 19:33:08 by hyungdki          #+#    #+#             */
/*   Updated: 2023/08/27 16:17:40 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

void	arg_heap_free(t_arg *arg)
{
	free(arg->last_eat_sem);
	free(arg->pid_lst);
}

void	kill_and_waitpid(t_arg *arg, int cnt)
{
	int	idx;

	idx = -1;
	while (++idx < cnt)
	{
		kill(arg->pid_lst[idx], SIGKILL);
		waitpid(arg->pid_lst[idx], T_NULL, 0);
	}	
}

void	arg_sems_destroy(t_arg *arg)
{
	int	idx;

	if (arg->fork_chk == TRUE)
		ft_sem_destroy(&arg->fork);
	if (arg->start_flag_chk == TRUE)
		ft_sem_destroy(&arg->start_flag);
	if (arg->print_sem_chk == TRUE)
		ft_sem_destroy(&arg->print_sem);
	idx = -1;
	while (++idx < arg->last_eat_sem_cnt)
	{
		free(arg->last_eat_sem[idx].name);
		ft_sem_destroy(&arg->last_eat_sem[idx]);
	}
}
