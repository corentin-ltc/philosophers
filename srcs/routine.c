/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-tort <cle-tort@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 16:41:32 by cle-tort          #+#    #+#             */
/*   Updated: 2024/09/16 17:12:53 by cle-tort         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	take_forks(t_philo *philo)
{
	pthread_mutex_lock(philo->right_fork);
	if (is_starved(philo, false))
	{
		pthread_mutex_unlock(philo->right_fork);
		return (true);
	}
	m_printf("has taken a fork\n", timenow(philo), *philo, false);
	pthread_mutex_lock(philo->left_fork);
	if (is_starved(philo, false))
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		return (true);
	}
	m_printf("has taken a fork\n", timenow(philo), *philo, false);
	return (false);
}

bool	eating(t_philo *philo)
{
	m_printf("is eating\n", timenow(philo), *philo, false);
	pthread_mutex_lock(&(philo->data->mutex[CHECK]));
	philo->last_meal = timenow(philo);
	pthread_mutex_unlock(&(philo->data->mutex[CHECK]));
	if (philo->data->time_to_eat > philo->data->time_to_die)
	{
		pthread_mutex_lock(&(philo->data->mutex[CHECK]));
		pthread_mutex_lock(&(philo->data->mutex[DEATH]));
		philo->data->someone_is_dead = true;
		pthread_mutex_unlock(&(philo->data->mutex[DEATH]));
		ft_usleep(philo->data->time_to_die * 1000);
		m_printf("died\n", timenow(philo), *philo, true);
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(&(philo->data->mutex[CHECK]));
		return (true);
	}
	ft_usleep(philo->data->time_to_eat * 1000);
	if (philo->data->max_meal_count != INFINI)
		philo->meal_count++;
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
	return (false);
}

bool	is_sleeping(t_philo *philo)
{
	m_printf("is sleeping\n", timenow(philo), *philo, false);
	if (timenow(philo) - philo->last_meal + philo->data->time_to_sleep
		> philo->data->time_to_die)
	{
		if (is_starved(philo, true))
			return (true);
		pthread_mutex_lock(&(philo->data->mutex[CHECK]));
		pthread_mutex_lock(&(philo->data->mutex[DEATH]));
		philo->data->someone_is_dead = true;
		pthread_mutex_unlock(&(philo->data->mutex[DEATH]));
		ft_usleep((philo->data->time_to_die
				- (timenow(philo) - philo->last_meal)) * 1000);
		m_printf("died\n", timenow(philo), *philo, true);
		pthread_mutex_unlock(&(philo->data->mutex[CHECK]));
	}
	ft_usleep(philo->data->time_to_sleep * 1000);
	return (false);
}

void	routine(t_philo *philo)
{
	m_printf("is thinking\n", timenow(philo), *philo, false);
	if (philo->name % 2 == 0)
		usleep(philo->data->time_to_eat * 900);
	while (!is_starved(philo, true) && (philo->meal_count
			< philo->data->max_meal_count)
		|| philo->data->max_meal_count == INFINI)
	{
		if (take_forks(philo))
			break ;
		pthread_mutex_lock(&philo->data->mutex[THINK]);
		philo->is_thinking = false;
		pthread_mutex_unlock(&philo->data->mutex[THINK]);
		if (eating(philo) || is_starved(philo, true) || is_sleeping(philo))
			break ;
		if (!is_starved(philo, true))
			m_printf("is thinking\n", timenow(philo), *philo, false);
		pthread_mutex_lock(&philo->data->mutex[THINK]);
		philo->is_thinking = true;
		pthread_mutex_unlock(&philo->data->mutex[THINK]);
		if (philo->data->nbr_of_philos % 2 && philo->data->time_to_eat
			>= philo->data->time_to_sleep)
			ft_usleep((philo->data->time_to_eat * 2
					- philo->data->time_to_sleep) * 1000);
	}
}

void	monitor(t_data *data)
{
	int	i;

	while ((get_bool(&(data->mutex[DEATH]), &data->someone_is_dead)) == false)
	{
		i = -1;
		pthread_mutex_lock(&(data->mutex[CHECK]));
		while (++i < data->nbr_of_philos)
		{
			if ((get_bool(&(data->mutex[THINK]), &data->philos[i].is_thinking)))
			{
				if (timenow(&data->philos[i]) - data->philos[i].last_meal
					> data->time_to_die)
				{
					m_printf("died\n", timenow(&data->philos[i]),
						data->philos[i], true);
					pthread_mutex_lock(&data->mutex[DEATH]);
					data->someone_is_dead = true;
					pthread_mutex_unlock(&data->mutex[DEATH]);
					break ;
				}
			}
		}
		pthread_mutex_unlock(&(data->mutex[CHECK]));
		usleep(1000);
	}
}
