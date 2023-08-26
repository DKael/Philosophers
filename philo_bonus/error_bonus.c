/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 22:06:11 by hyungdki          #+#    #+#             */
/*   Updated: 2023/08/26 21:19:20 by hyungdki         ###   ########.fr       */
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

int	check_end_flag(t_philo *data)
{
	int	return_value;

	sem_wait_nointr(data->end_flag_sem.sem);
	return_value = data->end_flag;
	sem_post(data->end_flag_sem.sem);
	return (return_value);
}
