/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 22:06:11 by hyungdki          #+#    #+#             */
/*   Updated: 2023/08/19 17:37:42 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static const char	*g_program_name;

void	err_init(char *argv)
{
	g_program_name = argv;
}

int	err_msg(const char *msg, int return_code)
{
	printf("%s: %s\n", g_program_name, msg);
	return (return_code);
}

void	arg_free(t_arg *arg)
{
	free(arg->philo);
	free(arg->fork);
	free(arg->last_eat_mtx);
	free(arg->log_mtx);
}

void *exit_thread(t_arg *arg, t_thread_status status, t_dll *dll)
{	
    if (arg->da_flag == 0)
    {
        if (status == DEATH)
            arg->da_flag |= DEATH;
        else if (status == ABORT)
            arg->da_flag |= ABORT;
    }
	if (dll != T_NULL)
		dll_clear(dll, log_delete_func);
    arg->end_flag++;
    return (T_NULL);
}

void	log_delete_func(void *content)
{
	t_log	*temp;

	if (content != T_NULL)
	{
		temp = (t_log *)content;
		free(temp);
	}
}
