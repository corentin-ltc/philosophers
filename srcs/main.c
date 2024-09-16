/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-tort <cle-tort@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 20:27:11 by cle-tort          #+#    #+#             */
/*   Updated: 2024/09/16 17:01:30 by cle-tort         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_data	data;

	if (!parsing(argc, argv, &data))
	{
		printf("Error\n");
		return (1);
	}
	init_data(&data);
	free(data.mutex);
	free(data.forks);
	free(data.philos);
	return (0);
}
