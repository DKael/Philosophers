/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resource_free_func_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 19:33:08 by hyungdki          #+#    #+#             */
/*   Updated: 2023/08/26 21:17:11 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"


void	kill_and_waitpid(t_arg *arg, int cnt)
{
	int	idx;

	idx = -1;
	while (++idx < cnt)
	{
		kill(arg->pid_lst[idx] ,SIGKILL);
		waitpid(arg->pid_lst[idx], T_NULL, 0);
	}	
}

void	arg_sems_destroy(t_arg *arg)
{
	if (arg->fork_chk == TRUE)
		ft_sem_destroy(&arg->fork);
	if (arg->start_flag_chk == TRUE)
		ft_sem_destroy(&arg->start_flag);
	if (arg->print_sem_chk == TRUE)
		ft_sem_destroy(&arg->print_sem_chk);
}
