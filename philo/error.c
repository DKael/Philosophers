/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 22:06:11 by hyungdki          #+#    #+#             */
/*   Updated: 2023/07/30 23:09:33 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static const char	*g_program_name;

void	err_init(char *argv)
{
	g_program_name = argv;
}

void	err_msg(const char *msg, int exit_code)
{
	printf("%s: %s\n", g_program_name, msg);
	exit(exit_code);
}

void	philo_free(t_philo *data)
{
	free(data->philo);
	free(data->fork);
}
