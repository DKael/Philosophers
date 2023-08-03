/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 20:36:06 by hyungdki          #+#    #+#             */
/*   Updated: 2023/08/03 18:46:47 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void* thread_function(void* arg)
{
    t_pass  *value;
    t_philo *data;
    int     idx;
    int     first_pick;
    int     second_pick;
    struct timeval time_lapse;
    int     eat_cnt;

    value = (t_pass *)arg;
    data = value->data;
    idx = value->idx;
    if (idx % 2 == 0)
    {
        first_pick = idx;
        second_pick = (idx + 1) % data->philo_num;
    }
    else
    {
        first_pick = (idx + 1) % data->philo_num;
        second_pick = idx;
    }
    eat_cnt = 0;
    while (data->s_flag == FALSE);
    while (1)
    {
        while (data->fork[first_pick] != -1);
        pthread_mutex_lock(&data->mutex);
        data->fork[first_pick] = idx;
        pthread_mutex_unlock(&data->mutex);
        while (data->fork[second_pick] != -1);
        pthread_mutex_lock(&data->mutex);
        data->fork[second_pick] = idx;
        pthread_mutex_unlock(&data->mutex);
        gettimeofday(&time_lapse, T_NULL);
        printf("%d ms %d has taken a fork\n", time_lapse.tv_usec - data->start.tv_usec, idx);
        gettimeofday(&time_lapse, T_NULL);
        printf("%d ms %d is eating\n", time_lapse.tv_usec - data->start.tv_usec, idx);
        usleep(data->e_time);
        data->fork[first_pick] = -1;
        data->fork[second_pick] = -1;
        if (++eat_cnt >= data->eat_cnt)
            break;
        gettimeofday(&time_lapse, T_NULL);
        printf("%d ms %d is sleeping\n", time_lapse.tv_usec - data->start.tv_usec, idx);
        usleep(data->s_time);
        gettimeofday(&time_lapse, T_NULL);
        printf("%d ms %d is thinkig\n", time_lapse.tv_usec - data->start.tv_usec, idx);
    }
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
    
    pass = (t_pass *)malloc(sizeof(t_pass) * data.philo_num);
    if (pass ==T_NULL)
    {
        philo_free(&data);
        err_msg("pthread create error!", 1);
    }
    idx = -1;
    while (++idx < data.philo_num)
        data.fork[idx] = -1;
    
    idx = -1;
    while (++idx < data.philo_num)
    {
        pass[idx].data = &data;
        pass[idx].idx = idx;
        if (pthread_create(&data.philo[idx], NULL, thread_function, &pass[idx]) != 0)
        {
            philo_free(&data);
            free(pass);
            err_msg("pthread create error!", 1);
        }    
    }
    gettimeofday(&data.start, T_NULL);
    data.s_flag = TRUE;

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




