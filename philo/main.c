/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 20:36:06 by hyungdki          #+#    #+#             */
/*   Updated: 2023/08/09 16:56:01 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void* thread_function(void* arg)
{
    t_philo  *value;
    t_philo *arg;
    int     idx;
    
    struct timeval time_lapse;
    int     eat_cnt;

    value = (t_philo *)arg;
    arg = value->arg;
    idx = value->idx;
    if (idx % 2 == 0)
    {
        first_pick = idx;
        second_pick = (idx + 1) % arg->philo_num;
    }
    else
    {
        first_pick = (idx + 1) % arg->philo_num;
        second_pick = idx;
    }
    eat_cnt = 0;
    while (arg->s_flag == FALSE);
    while (1)
    {
        while (arg->fork[first_pick] != -1);
        pthread_mutex_lock(&arg->mutex);
        arg->fork[first_pick] = idx;
        pthread_mutex_unlock(&arg->mutex);
        while (arg->fork[second_pick] != -1);
        pthread_mutex_lock(&arg->mutex);
        arg->fork[second_pick] = idx;
        pthread_mutex_unlock(&arg->mutex);
        gettimeofday(&time_lapse, T_NULL);
        printf("%d ms %d has taken a fork\n", time_lapse.tv_usec - arg->start.tv_usec, idx);
        gettimeofday(&time_lapse, T_NULL);
        printf("%d ms %d is eating\n", time_lapse.tv_usec - arg->start.tv_usec, idx);
        usleep(arg->e_time);
        arg->fork[first_pick] = -1;
        arg->fork[second_pick] = -1;
        if (++eat_cnt >= arg->eat_cnt)
            break;
        gettimeofday(&time_lapse, T_NULL);
        printf("%d ms %d is sleeping\n", time_lapse.tv_usec - arg->start.tv_usec, idx);
        usleep(arg->s_time);
        gettimeofday(&time_lapse, T_NULL);
        printf("%d ms %d is thinkig\n", time_lapse.tv_usec - arg->start.tv_usec, idx);
    }
    return (T_NULL);
}

int main(int argc, char **argv)
{
    t_arg arg; 

    if (argc != 5 && argc != 6)
    {
        printf("Usage : %s number_of_philosophers \
        time_to_die time_to_eat time_to_sleep \
        [number_of_times_each_philosopher_must_eat]\n", argv[0]);
        return (1);
    }
    err_init(argv[0]);
    arg_init(&arg, argc, argv);
    arg.philo = (pthread_t *)ft_calloc(arg.philo_num, sizeof(pthread_t));
    if (arg.philo == T_NULL)
        err_msg("malloc error!", 1);
	arg.fork = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * arg.philo_num);
    if (arg.fork == T_NULL)
    {
        free(arg.philo);
        err_msg("malloc error!", 1);
    }





    int idx;
    
    pthread_mutex_init(&arg.mutex,NULL);

    t_philo  *philo;
    int idx;
    
    philo = (t_philo *)malloc(sizeof(t_philo) * arg.philo_num);
    if (philo ==T_NULL)
    {
        philo_free(&arg);
        err_msg("pthread create error!", 1);
    }
    idx = -1;
    while (++idx < arg.philo_num)
        arg.fork[idx] = -1;
    
    idx = -1;
    while (++idx < arg.philo_num)
    {
        philo[idx].arg = &arg;
        philo[idx].idx = idx;
        if (pthread_create(&arg.philo[idx], NULL, thread_function, &philo[idx]) != 0)
        {
            philo_free(&arg);
            free(philo);
            err_msg("pthread create error!", 1);
        }    
    }
    gettimeofday(&arg.start, T_NULL);
    arg.s_flag = TRUE;

    idx = -1;
    while (++idx < arg.philo_num)
    {
        if (pthread_join(arg.philo[idx], NULL) != 0)
        {
            philo_free(&arg);
            free(philo);
            err_msg("pthread join error\n", 1);
        }   
    }
    
    pthread_mutex_destroy(&arg.mutex);
    free(philo);
    philo_free(&arg);
    return 0;
}




