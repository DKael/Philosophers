/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 22:06:11 by hyungdki          #+#    #+#             */
/*   Updated: 2023/08/14 11:17:58 by hyungdki         ###   ########.fr       */
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
}
