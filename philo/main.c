/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 20:36:06 by hyungdki          #+#    #+#             */
/*   Updated: 2023/07/30 21:50:05 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void* thread_function(void* arg)
{
    int* value = (int*)arg;
	int return_value = 100;
    printf("Child thread: received value = %d\n", *value);
    *value = 42; // 자식 스레드에서 반환할 값 설정
    pthread_exit((void *)&return_value);
}

int main(int argc, char **argv)
{
    pthread_t thread_id[5];
    int value = 10;
    
    // 자식 스레드 생성
    if (pthread_create(&thread_id, NULL, thread_function, &value) != 0) {
        perror("pthread_create");
        return 1;
    }
    
    // 자식 스레드가 종료될 때까지 기다림
    void* retval;
    if (pthread_join(thread_id, &retval) != 0) {
        perror("pthread_join");
        return 1;
    }
    
    printf("Parent thread: child thread exited with value = %d\n", (int*)retval);
    
    return 0;
}

int	ft_atoi_int(const char *str)
{
	int	idx;
	int	sign;
	int	nb;

	idx = 0;
	sign = 1;
	nb = 0;
	while ((9 <= str[idx] && str[idx] <= 13) || str[idx] == ' ')
		idx++;
	if (str[idx] == '-' || str[idx] == '+')
		if (str[idx++] == '-')
			sign = -1;
	while ('0' <= str[idx] && str[idx] <= '9')
	{
		if (nb > INT_MAX / 10
			|| (nb == (INT_MAX / 10)
				&& str[idx] - '0' > (INT_MAX % 10) - ((sign - 1) / 2)))
			return (0);
		nb = nb * 10 + (str[idx++] - '0');
	}
	return ((int)nb * sign);
}
