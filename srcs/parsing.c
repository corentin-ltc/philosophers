/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-tort <cle-tort@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 16:58:34 by cle-tort          #+#    #+#             */
/*   Updated: 2024/09/16 17:03:20 by cle-tort         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (true);
	else
		return (false);
}

int	parsing(int argc, char **argv, t_data *data)
{
	if (!are_arguments_valids(argc, argv))
		return (0);
	data->nbr_of_philos = ft_atol(argv[1]);
	data->time_to_die = ft_atol(argv[2]);
	data->time_to_eat = ft_atol(argv[3]);
	data->time_to_sleep = ft_atol(argv[4]);
	if (data->nbr_of_philos == -42 || data->time_to_die == -42
		|| data->time_to_eat == -42 || data->time_to_sleep == -42)
		return (0);
	if (argc == 6)
	{
		data->max_meal_count = ft_atol(argv[5]);
		if (data->max_meal_count == -42)
			return (0);
	}
	else
		data->max_meal_count = INFINI;
	data->philos = malloc(sizeof(t_philo) * data->nbr_of_philos);
	if (!data->philos)
		return (0);
	return (1);
}

long	ft_atol(char *str)
{
	long	nb;
	long	a;
	int		i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			return (-42);
		i++;
	}
	nb = 0;
	a = 0;
	while (ft_isdigit(str[i]))
	{
		a = str[i] - '0';
		nb *= 10;
		nb = nb + a;
		if (nb > INT_MAX)
			return (-42);
		i++;
	}
	return (nb);
}

bool	str_is_valid_number(char *str)
{
	int		i;
	bool	has_digit;

	has_digit = false;
	i = 0;
	while (str[i])
	{
		if (i == 0 && str[i] == '+')
			break ;
		if (str[i] < '0' || str[i] > '9')
			return (false);
		else
			has_digit = true;
		i++;
	}
	return (has_digit);
}

bool	are_arguments_valids(int argc, char **args)
{
	int	i;

	if (argc < 5 || argc > 6)
	{
		printf("Wrong number of arguments\n");
		return (false);
	}
	i = 1;
	while (i < argc)
	{
		if (!str_is_valid_number(args[i]))
		{
			printf("%s is not a valid number\n", args[i]);
			return (false);
		}
		i++;
	}
	return (true);
}
