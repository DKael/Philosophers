/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 22:06:11 by hyungdki          #+#    #+#             */
/*   Updated: 2023/08/25 13:31:17 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

int	err_msg(t_arg *arg, const char *msg, int return_code)
{
	if (msg != T_NULL)
		printf("%s: %s\n", arg->program_name, msg);
	else
		printf("%s: undefined error\n", arg->program_name);
	return (return_code);
}

void	*thread_error_end(t_arg *arg)
{
	sem_wait_nointr(arg->end_flag_sem.sem);
	if (arg->end_flag == NORMAL)
		arg->end_flag = ABORT;
	sem_post(arg->end_flag_sem.sem);
	return (T_NULL);
}

int	check_end_flag(t_arg *arg)
{
	int	return_value;

	sem_wait_nointr(arg->end_flag_sem.sem);
	return_value = arg->end_flag;
	sem_post(arg->end_flag_sem.sem);
	return (return_value);
}
