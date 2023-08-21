/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 20:36:03 by hyungdki          #+#    #+#             */
/*   Updated: 2023/08/21 16:45:52 by hyungdki         ###   ########.fr       */
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
#ifndef NULL
#define NULL (void *)0
#endif

#define S_TO_US 1000000
#define MS_TO_US 1000

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
	int fork_cnt;

	pthread_mutex_t *last_eat_mtx;
	int last_eat_mtx_cnt;

	pthread_mutex_t *log_mtx;
	int log_mtx_cnt;

	pthread_mutex_t start_flag;
	t_bool start_flag_chk;

	pthread_mutex_t end_flag_mtx;
	t_bool end_flag_mtx_chk;
	t_bool end_flag;

	pthread_t print_thrd;
	pthread_t time_thrd;
	t_timeval start;

	char *program_name;
} t_arg;

typedef struct s_philo
{
	int idx;
	pthread_mutex_t *first_fork;
	pthread_mutex_t *second_fork;
	long last_eat;
	pthread_t thrd;
	t_dll logs;
	t_arg *arg;
	int eat_cnt;
} t_philo;

typedef enum e_philo_status
{
	THINKING,
	GET_FORK,
	EATING,
	SLEEPING,
	DIE
} t_philo_status;

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
	t_philo_status status;
	t_dllnode *ptr;
} t_srt;

int check_end_flag(t_arg *arg);

int err_msg(t_arg *arg, const char *msg, int return_code);
void arg_free(t_arg *data);
void log_delete_func(void *content);
void	*thread_error_end(t_arg *arg);
void philos_log_clear(t_arg *arg, int cnt);

void *philo_thread_func(void *arg);
void *print_thread_func(void *arg);
void *time_thread_func(void *arg);
t_bool report(t_philo *value, t_philo_status status, t_arg *arg);

int arg_init(t_arg *data, int argc, char **argv);
int ft_atoi_int(const char *str);
t_bool ft_isdecimal(char *str);
char *ft_itoa(int n);
void *ft_calloc(size_t count, size_t size);
t_bool ft_usleep(long us);

#endif