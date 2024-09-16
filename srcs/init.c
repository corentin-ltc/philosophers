/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-tort <cle-tort@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 16:07:38 by cle-tort          #+#    #+#             */
/*   Updated: 2024/09/16 16:32:40 by cle-tort         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_philo(t_philo *philo, t_data *data)
{
	pthread_mutex_t	*tmp;

	philo->last_meal = timenow(philo);
	philo->meal_count = 0;
	philo->data = data;
	philo->is_thinking = true;
	philo->left_fork = &data->forks[philo->name - 1];
	if (philo->name == data->nbr_of_philos)
		philo->right_fork = &data->forks[0];
	else
		philo->right_fork = &data->forks[philo->name];
	if (philo->name % 2)
	{
		tmp = philo->right_fork;
		philo->right_fork = philo->left_fork;
		philo->left_fork = tmp;
	}
	pthread_create(&(philo->thread), NULL, (void *)routine, philo);
}

int	init_data(t_data *data)
{
	int				i;
	struct timeval	tv;
	long long		time_in_ms;

	data->someone_is_dead = false;
	gettimeofday(&tv, NULL);
	time_in_ms = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	data->mutex = malloc(sizeof(pthread_mutex_t) * 5);
	if (!data->mutex)
		return (1);
	i = 0;
	while (i < 5)
		pthread_mutex_init(&data->mutex[i++], NULL);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->nbr_of_philos);
	if (!data->forks)
	{
		free(data->mutex);
		return (1);
	}
	i = 0;
	while (i < data->nbr_of_philos)
		pthread_mutex_init(&(data->forks[i++]), NULL);
	if (data->nbr_of_philos == 1)
		return (one_philo(data));
	return (start_threads(data, time_in_ms));
}

int	one_philo(t_data *data)
{
	printf("0 0 has taken a fork\n");
	ft_usleep(data->time_to_die * 1000);
	printf("%lld 0 died\n", (long long) data->time_to_die);
	return (0);
}

int	start_threads(t_data *data, long long time_in_ms)
{
	size_t	i;

	i = 0;
	while (i < data->nbr_of_philos)
	{
		data->philos[i].start_time = time_in_ms;
		data->philos[i].name = i + 1;
		init_philo(&(data->philos[i]), data);
		i++;
	}
	pthread_create(&(data->monitor_thread), NULL, (void *)monitor, data);
	i = 0;
	while (i < data->nbr_of_philos)
	{
		pthread_join(data->philos[i].thread, NULL);
		i++;
	}
	pthread_mutex_lock(&data->mutex[DEATH]);
	data->someone_is_dead = true;
	pthread_mutex_unlock(&data->mutex[DEATH]);
	pthread_join(data->monitor_thread, NULL);
	return (0);
}
