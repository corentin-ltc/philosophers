/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-tort <cle-tort@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 20:27:11 by cle-tort          #+#    #+#             */
/*   Updated: 2024/09/14 22:01:13 by cle-tort         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void secure_printf(char *message, long long time, t_philo philo, bool is_death)
{
    pthread_mutex_lock(&(philo.data->mutex[TEXT]));
	if (!is_death)
	{
		if (!philo.data->someone_is_dead)
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

bool is_starved(t_philo *philo, bool just_checking)
{
    long long now = timenow(philo);
    pthread_mutex_lock(&(philo->data->mutex[CHECK]));
    if (philo->data->someone_is_dead)
    {
        pthread_mutex_unlock(&(philo->data->mutex[CHECK]));
        return (true);
    }
    if (!just_checking && (now - 2 - philo->time_of_last_meal > philo->data->time_to_die))
    {
		philo->data->someone_is_dead = true;
	    pthread_mutex_unlock(&(philo->data->mutex[CHECK]));
        secure_printf("died\n", now, *philo, true);
        return (true);
    }
    pthread_mutex_unlock(&(philo->data->mutex[CHECK]));
    return (false);
}


bool modulo_zero(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	if (is_starved(philo, false))
	{
		pthread_mutex_unlock(philo->left_fork);	
		return (true);
	}
	secure_printf("has taken a fork\n", timenow(philo), *philo, false);
	pthread_mutex_lock(philo->right_fork);
	if (is_starved(philo, false))
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		return (true);
	}
	secure_printf("has taken a fork\n", timenow(philo), *philo, false);
	return (false);
}
bool modulo_one(t_philo *philo)
{
	pthread_mutex_lock(philo->right_fork);
	if (is_starved(philo, false))
	{
		pthread_mutex_unlock(philo->right_fork);
		return (true);
	}
	secure_printf("has taken a fork\n", timenow(philo), *philo, false);
	pthread_mutex_lock(philo->left_fork);
	if (is_starved(philo, false))
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);					
		return (true);
	}
	secure_printf("has taken a fork\n", timenow(philo), *philo, false);
	return (false);
}
bool eating(t_philo *philo)
{
	secure_printf("is eating\n", timenow(philo), *philo, false);
	philo->time_of_last_meal = timenow(philo);
	if (philo->data->time_to_eat > philo->data->time_to_die)
	{
		pthread_mutex_lock(&(philo->data->mutex[CHECK]));
		philo->data->someone_is_dead = true;
		usleep(philo->data->time_to_die * 1000);
	    secure_printf("died\n", timenow(philo), *philo, true);
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(&(philo->data->mutex[CHECK]));
		return (true);
	}
	usleep(philo->data->time_to_eat * 1000);
	if (philo->data->max_meal_count != INFINI)
		philo->meal_count++;
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	return (false);
}

bool is_sleeping(t_philo *philo)
{
	secure_printf("is sleeping\n", timenow(philo), *philo, false);
	if (timenow(philo) - philo->time_of_last_meal + philo->data->time_to_sleep > philo->data->time_to_die)
	{
		if (is_starved(philo, true))
			return (true);
		pthread_mutex_lock(&(philo->data->mutex[CHECK]));
		philo->data->someone_is_dead = true;
		usleep((philo->data->time_to_die - (timenow(philo) - philo->time_of_last_meal)) * 1000);
		secure_printf("died\n", timenow(philo), *philo, true);
		pthread_mutex_unlock(&(philo->data->mutex[CHECK]));
	}
	usleep(philo->data->time_to_sleep * 1000);
	return (false);
}

void routine(t_philo *philo)
{
	secure_printf("is thinking\n", timenow(philo), *philo, false);		
	if (philo->name % 2)
		usleep(philo->data->time_to_eat);
  	while(!is_starved(philo, true) && (philo->meal_count < philo->data->max_meal_count) || philo->data->max_meal_count == INFINI)
 	{
		if (philo->data->number_of_philosophers % 2 || philo->name % 2)
		{
			if (modulo_one(philo))
				break;
		}
		else
		{
			if (modulo_zero(philo))
				break;
		}
		if (eating(philo))
				break;
		if (is_starved(philo, true) || is_sleeping(philo))
			break;
		if (!is_starved(philo, true))
			secure_printf("is thinking\n", timenow(philo), *philo, false);		
	}
}


void init_philo(t_philo *philo, t_data *data)
{
	
    philo->time_of_last_meal = timenow(philo);
    philo->meal_count = 0;
	philo->data = data;
    if (philo->name == data->number_of_philosophers - 1)
    {
        philo->left_fork = &data->forks[philo->name];
        philo->right_fork = &data->forks[0];
    }
    else
    {
        philo->left_fork = &data->forks[philo->name];
        philo->right_fork = &data->forks[philo->name + 1];
    }
	pthread_create(&(philo->thread), NULL, (void *)routine, philo);
}

void init_data(t_data *data)
{
    int         i;
    struct timeval tv;
    long long      time_in_ms;

	data->someone_is_dead = false;
    gettimeofday(&tv, NULL);
    time_in_ms = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);

	data->mutex = malloc(sizeof(pthread_mutex_t) * 5);
	if (!data->mutex)
	{
		// free philos
		return ;
	}
	i = 0;
	while (i < 5)
		pthread_mutex_init(&data->mutex[i++], NULL);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->number_of_philosophers);
	if (!data->forks)
	{
		// free philos
		return ;
	}
	i = 0;
	while (i < data->number_of_philosophers)
		pthread_mutex_init(&(data->forks[i++]), NULL);
    if (data->number_of_philosophers == 1)
    {
      	printf("0 0 has taken a fork\n");
		usleep(data->time_to_die * 1000);
		printf("%lld 0 died\n", (long long) data->time_to_die);
		return ;
    }
    i = 0;
    while (i < data->number_of_philosophers)
    {
		data->philos[i].start_time = time_in_ms;
        data->philos[i].name = i;
        init_philo(&(data->philos[i]), data);
        i++;
    }
    i = 0;
    while (i < data->number_of_philosophers)
    {
        pthread_join(data->philos[i].thread, NULL);
        i++;
	}
	free(data->mutex);
	free(data->forks);
	free(data->philos);
}

int parsing(int argc, char **argv, t_data *data)
{
    if (!are_arguments_valids(argc, argv))
        return (0);
    data->number_of_philosophers = ft_atol(argv[1]);
    data->time_to_die = ft_atol(argv[2]);
    data->time_to_eat = ft_atol(argv[3]);
    data->time_to_sleep = ft_atol(argv[4]);
    data->max_meal_count = (argc == 6) ? ft_atol(argv[5]) : INFINI;
    data->philos = malloc(sizeof(t_philo) * data->number_of_philosophers);
	if (!data->philos)
		return (0);
    return (1);
}

int main(int argc, char **argv)
{
    t_data  data;

    if (!parsing(argc, argv, &data))
        return (0);
    init_data(&data);

    return (0);
}
