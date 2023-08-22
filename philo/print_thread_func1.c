/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_thread_func1.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 16:58:56 by hyungdki          #+#    #+#             */
/*   Updated: 2023/08/22 22:33:01 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

extern int	print_thread_func2(t_arg *arg, long time_offset);

void	*print_thread_func(void *input_arg)
{
	t_arg	*arg;
	long	time_offset;

	arg = (t_arg *)input_arg;
	if (arg->philo_num < 100)
		time_offset = 500;
	else if (arg->philo_num >= 200)
		time_offset = 1000;
	else
		time_offset = arg->philo_num * 5;
	pthread_mutex_lock(&arg->start_flag);
	pthread_mutex_unlock(&arg->start_flag);
	if (usleep(time_offset) == EINTR)
		printf("Interrupted by a signa\n");
	if (print_thread_func2(arg, time_offset) == 1)
		return (thread_error_end(arg));
	return (T_NULL);
}
