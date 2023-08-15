/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 20:36:03 by hyungdki          #+#    #+#             */
/*   Updated: 2023/08/15 23:24:33 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
#define PHILOSOPHERS_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <memory.h>
#include <unistd.h>
#include <sys/time.h>
#include <limits.h>
#include "double_linked_list.h"
#include "ft_errno.h"
#include "quick_sort.h"

#if !defined(TRUE) && !defined(FALSE)
#define TRUE 1
#define FALSE 0
#endif
#ifndef T_NULL
#define T_NULL (void *)0
#endif

typedef int t_bool;

typedef struct timeval t_timeval;
typedef struct s_arg
{
	int philo_num;
	int d_time;
	int e_time;
	int s_time;
	int have_to_eat;
	struct s_philo *philo;
	pthread_mutex_t *fork;
	pthread_t print_thrd;
	pthread_t time_thrd;
	t_timeval start;
	long start_usec;
	t_timeval now;
	long now_usec;
	t_bool start_flag;
	int da_flag;
	int end_flag;
	int errno;
} t_arg;

typedef struct s_philo
{
	int idx;
	pthread_mutex_t *first_fork;
	pthread_mutex_t *second_fork;
	t_timeval last_eat;
	pthread_t thrd;
	t_dll logs;
	t_arg *arg;
	int eat_cnt;
} t_philo;

typedef enum e_philo_status
{
	GET_FORK,
	EATING,
	SLEEPING,
	THINKING,
	DIE
} t_philo_status;

typedef enum e_thread_status
{
	DEATH = 1,
	ABORT = 2
} t_thread_status;

typedef struct s_log
{
	t_timeval time;
	long usec;
	int who;
	t_philo_status status;
} t_log;

typedef struct s_srt
{
	long usec;
	t_dllnode *ptr;
} t_srt;

void err_init(char *argv);
int err_msg(const char *msg, int return_code);
void arg_free(t_arg *data);
void *exit_thread(t_arg *arg, t_thread_status status, t_dll *dll);
void log_delete_func(void *content);

void *philo_thread_func(void *arg);
void *print_thread_func(void *arg);
void *time_thread_func(void *arg);
t_bool report(t_philo *value, t_philo_status status, t_timeval start);

int arg_init(t_arg *data, int argc, char **argv);
int ft_atoi_int(const char *str);
t_bool ft_isdecimal(char *str);
char *ft_itoa(int n);
void *ft_calloc(size_t count, size_t size);

#endif