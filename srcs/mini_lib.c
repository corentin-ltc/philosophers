/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_lib.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-tort <cle-tort@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 17:01:32 by cle-tort          #+#    #+#             */
/*   Updated: 2024/09/16 17:12:40 by cle-tort         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	m_printf(char *message, long long time, t_philo philo, bool is_death)
{
	pthread_mutex_lock(&(philo.data->mutex[TEXT]));
	if (!is_death)
	{
		if ((get_bool(&(philo.data->mutex[DEATH]),
					&philo.data->someone_is_dead)) == false)
			printf("%lld %d %s", time, philo.name, message);
	}
	else
		printf("%lld %d %s", time, philo.name, message);
	pthread_mutex_unlock(&(philo.data->mutex[TEXT]));
}
