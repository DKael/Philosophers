/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 20:36:06 by hyungdki          #+#    #+#             */
/*   Updated: 2023/07/30 23:16:00 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void* thread_function(void* arg)
{
    t_pass *value;

    value = (t_pass *)arg;
    printf("idx : %d, left_first : %s\n", value->idx, value->left_first ? "True" : "false");

    return (T_NULL);
}

int main(int argc, char **argv)
{
    t_philo data;

    if (argc != 5 && argc != 6)
    {
        printf("Usage : %s number_of_philosophers \
        time_to_die time_to_eat time_to_sleep \
        [number_of_times_each_philosopher_must_eat]\n", argv[0]);
        return (1);
    }
    err_init(argv[0]);
    philo_init(&data);
    arg_init(&data, argc, argv, 0);
    data.philo = (pthread_t *)malloc(sizeof(pthread_t) * data.philo_num);
    if (data.philo == T_NULL)
        err_msg("malloc error!", 1);
    data.fork = (int *)malloc(sizeof(int) * data.philo_num);
    if (data.fork == T_NULL)
    {
        free(data.philo);
        err_msg("malloc error!", 1);
    }
    pthread_mutex_init(&data.mutex,NULL);

    t_pass  *pass;
    int idx;
    t_bool left_first;
    
    pass = (t_pass *)malloc(sizeof(t_pass) * data.philo_num);
    if (pass ==T_NULL)
    {
        philo_free(&data);
        err_msg("pthread create error!", 1);
    }    
    idx = -1;
    left_first = FALSE;
    while (++idx < data.philo_num)
    {
        pass[idx].data = &data;
        pass[idx].idx = idx;
        left_first = !left_first;
        pass[idx].left_first = !left_first;
        if (pthread_create(&data.philo[idx], NULL, thread_function, &pass[idx]) != 0)
        {
            philo_free(&data);
            free(pass);
            err_msg("pthread create error!", 1);
        }    
    }


    idx = -1;
    while (++idx < data.philo_num)
    {
        if (pthread_join(data.philo[idx], NULL) != 0)
        {
            philo_free(&data);
            free(pass);
            err_msg("pthread join error\n", 1);
        }   
    }
    
    pthread_mutex_destroy(&data.mutex);
    free(pass);
    philo_free(&data);
    return 0;
}




