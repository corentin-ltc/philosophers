/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-tort <cle-tort@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 17:16:06 by cle-tort          #+#    #+#             */
/*   Updated: 2024/09/15 22:40:15 by cle-tort         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void secure_printf(char *message, long long time, t_philo philo, bool is_death)
{
    pthread_mutex_lock(&(philo.data->mutex[TEXT]));
	if (!is_death)
	{
		if ((get_bool(&(philo.data->mutex[DEATH]), &philo.data->someone_is_dead)) == false)
			printf("%lld %d %s", time, philo.name, message);
	}
	else
		printf("%lld %d %s", time, philo.name, message);
    pthread_mutex_unlock(&(philo.data->mutex[TEXT]));
}

long long timenow(t_philo *philo)
{
    struct timeval tv;
    long long      time_in_ms;

    gettimeofday(&tv, NULL);
    time_in_ms = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
    return (time_in_ms - philo->start_time);
}

bool get_bool(pthread_mutex_t *mutex, bool *boolean)
{
		bool turbool;

		pthread_mutex_lock((mutex));
		turbool = *boolean;
		pthread_mutex_unlock((mutex));
		return (turbool);
}

void ft_usleep(long long microseconds)
{
	struct timeval tv;
    long long      time_in_ms;
    long long      start_time_in_ms;

    gettimeofday(&tv, NULL);
    time_in_ms = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	start_time_in_ms = time_in_ms;
	while (time_in_ms < (start_time_in_ms + microseconds / 1000))
	{
		usleep(100);
		gettimeofday(&tv, NULL);
    	time_in_ms = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	}
}	