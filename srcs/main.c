/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-tort <cle-tort@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 20:27:11 by cle-tort          #+#    #+#             */
/*   Updated: 2024/09/15 17:42:28 by cle-tort         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool is_starved(t_philo *philo, bool just_checking)
{
    long long now = timenow(philo);
    pthread_mutex_lock(&(philo->data->mutex[CHECK]));
    if ((get_bool(&(philo->data->mutex[DEATH]), &philo->data->someone_is_dead)))
    {
        pthread_mutex_unlock(&(philo->data->mutex[CHECK]));
        return (true);
    }
    if (!just_checking && (now - 2 - philo->time_of_last_meal > philo->data->time_to_die))
    {
   		pthread_mutex_lock(&(philo->data->mutex[DEATH]));
		philo->data->someone_is_dead = true;
   		pthread_mutex_unlock(&(philo->data->mutex[DEATH]));
	    pthread_mutex_unlock(&(philo->data->mutex[CHECK]));
        secure_printf("died\n", now, *philo, true);
        return (true);
    }
    pthread_mutex_unlock(&(philo->data->mutex[CHECK]));
    return (false);
}

bool take_forks(t_philo *philo)
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
	pthread_mutex_lock(&(philo->data->mutex[CHECK]));
	philo->time_of_last_meal = timenow(philo);
	pthread_mutex_unlock(&(philo->data->mutex[CHECK]));
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
   		pthread_mutex_lock(&(philo->data->mutex[DEATH]));
		philo->data->someone_is_dead = true;
   		pthread_mutex_unlock(&(philo->data->mutex[DEATH]));
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
   		pthread_mutex_lock(&philo->data->mutex[THINK]);
		philo->is_thinking = true;
   		pthread_mutex_unlock(&philo->data->mutex[THINK]);
		if (take_forks(philo))
			break;
   		pthread_mutex_lock(&philo->data->mutex[THINK]);
		philo->is_thinking = false;
   		pthread_mutex_unlock(&philo->data->mutex[THINK]);
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
	pthread_mutex_t	*tmp;
	
    philo->time_of_last_meal = timenow(philo);
    philo->meal_count = 0;
	philo->data = data;
	philo->is_thinking = true;
	philo->left_fork = &data->forks[philo->name - 1];
    if (philo->name == data->number_of_philosophers)
        philo->right_fork = &data->forks[0];
    else
        philo->right_fork = &data->forks[philo->name];
	if (philo->data->number_of_philosophers % 2 || philo->name % 2 == 0 )
	{
		tmp = philo->right_fork;
		philo->right_fork = philo->left_fork;
        philo->left_fork = tmp;
	}
	pthread_create(&(philo->thread), NULL, (void *)routine, philo);
}



void monitor(t_data *data)
{
	int	i;

	while ((get_bool(&(data->mutex[DEATH]), &data->someone_is_dead)) == false)
	{	
		i = 0;
		pthread_mutex_lock(&(data->mutex[CHECK]));
		while (i < data->number_of_philosophers)
		{
			if ((get_bool(&(data->mutex[THINK]), &data->philos[i].is_thinking)))
			{
			//	printf("philo %d : %lld\n", i + 1, timenow(&data->philos[i]) - data->philos[i].time_of_last_meal);
				if (timenow(&data->philos[i]) - data->philos[i].time_of_last_meal > data->time_to_die)
				{
					secure_printf("died\n", timenow(&data->philos[i]), data->philos[i], true);
   					pthread_mutex_lock(&data->mutex[DEATH]);
					data->someone_is_dead = true;
			   		pthread_mutex_unlock(&data->mutex[DEATH]);
					break;
				}
			}
			i++;
		}
		pthread_mutex_unlock(&(data->mutex[CHECK]));
		usleep(100);
	}
}

void init_data(t_data *data)
{
    int         i;
    struct timeval tv;
    long long      time_in_ms;

	data->someone_is_dead = false;
    gettimeofday(&tv, NULL);
    time_in_ms = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);

	data->mutex = malloc(sizeof(pthread_mutex_t) * 4);
	if (!data->mutex)
	{
		// free philos
		return ;
	}
	i = 0;
	while (i < 4)
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
        data->philos[i].name = i + 1;
        init_philo(&(data->philos[i]), data);
        i++;
    }
	// init monitor
	pthread_create(&(data->monitor_thread), NULL, (void *)monitor, data);
    i = 0;
    while (i < data->number_of_philosophers)
    {
        pthread_join(data->philos[i].thread, NULL);
        i++;
	}
	pthread_mutex_lock(&data->mutex[DEATH]);
	data->someone_is_dead = true;
	pthread_mutex_unlock(&data->mutex[DEATH]);
    pthread_join(data->monitor_thread, NULL);
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
