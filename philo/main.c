/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 20:36:06 by hyungdki          #+#    #+#             */
/*   Updated: 2023/08/23 07:48:34 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// void	leaks(void)
// {
// 	system("leaks --list -- main");
// }

int	main(int argc, char **argv)
{
	int	result;

	if (argc != 5 && argc != 6)
	{
		printf("Usage : %s number_of_philosophers \
time_to_die time_to_eat time_to_sleep \
[number_of_times_each_philosopher_must_eat]\n", argv[0]);
		return (1);
	}
	result = philosopher_start(argc, argv);
	return (result);
}
