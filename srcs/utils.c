/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-tort <cle-tort@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 17:16:06 by cle-tort          #+#    #+#             */
/*   Updated: 2024/09/16 17:09:32 by cle-tort         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	timenow(t_philo *philo)
{
	struct timeval	tv;
	long long		time_in_ms;	

	gettimeofday(&tv, NULL);
	time_in_ms = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (time_in_ms - philo->start_time);
}

bool	get_bool(pthread_mutex_t *mutex, bool *boolean)
{
	bool	turbool;

	pthread_mutex_lock((mutex));
	turbool = *boolean;
	pthread_mutex_unlock((mutex));
	return (turbool);
}

void	ft_usleep(long long microseconds)
{
	struct timeval		tv;
	long long			time_in_ms;
	long long			start_time_in_ms;

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

bool	is_starved(t_philo *philo, bool just_checking)
{
	long long	now;

	now = timenow(philo);
	pthread_mutex_lock(&(philo->data->mutex[CHECK]));
	if ((get_bool(&(philo->data->mutex[DEATH]), &philo->data->someone_is_dead)))
	{
		pthread_mutex_unlock(&(philo->data->mutex[CHECK]));
		return (true);
	}
	if (!just_checking && (now - philo->last_meal > philo->data->time_to_die))
	{
		pthread_mutex_lock(&(philo->data->mutex[DEATH]));
		philo->data->someone_is_dead = true;
		pthread_mutex_unlock(&(philo->data->mutex[DEATH]));
		pthread_mutex_unlock(&(philo->data->mutex[CHECK]));
		m_printf("died\n", now, *philo, true);
		return (true);
	}
	pthread_mutex_unlock(&(philo->data->mutex[CHECK]));
	return (false);
}
