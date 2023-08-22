/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 22:06:11 by hyungdki          #+#    #+#             */
/*   Updated: 2023/08/21 19:33:55 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int err_msg(t_arg *arg, const char *msg, int return_code)
{
	if (msg != T_NULL)
		printf("%s: %s\n", arg->program_name, msg);
	else
		printf("%s: undefined error\n", arg->program_name);
	return (return_code);
}

void	*thread_error_end(t_arg *arg)
{
	pthread_mutex_lock(&arg->end_flag_mtx);
	arg->end_flag = TRUE;
	pthread_mutex_unlock(&arg->end_flag_mtx);
	return (T_NULL);
}