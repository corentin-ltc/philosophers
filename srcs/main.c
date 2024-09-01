#include "philo.h"

void eating(t_philo *philo)
{
	struct timeval			tv;
	long long					time_in_ms;
	
	while (philo->max_meal_count == INFINI || philo->meal_count < philo->max_meal_count)
	{
		if (philo->name % 2 == 0)
		{
			pthread_mutex_lock(philo->left_fork);
			gettimeofday(&tv, NULL);
			time_in_ms = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ;
			printf("%lld %d has taken a fork\n", (long long)time_in_ms, philo->name);	
			pthread_mutex_lock(philo->right_fork);
			gettimeofday(&tv, NULL);
			time_in_ms = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ;
			printf("%lld %d has taken a fork\n", (long long)time_in_ms, philo->name);
		}
		else 
		{
			pthread_mutex_lock(philo->right_fork);
			gettimeofday(&tv, NULL);
			time_in_ms = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ;
			printf("%lld %d has taken a fork\n", (long long)time_in_ms, philo->name);
			pthread_mutex_lock(philo->left_fork);
			gettimeofday(&tv, NULL);
			time_in_ms = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ;
			printf("%lld %d has taken a fork\n", (long long)time_in_ms, philo->name);
		}
		
		/*----------------EATING-----------------*/

		gettimeofday(&tv, NULL);
		time_in_ms = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ;
		printf("%lld %d is eating\n", (long long)time_in_ms, philo->name);
		usleep(philo->time_to_eat);
		if (philo->max_meal_count != INFINI)
			philo->meal_count++;
			
		/*----------------END OF EATING-----------------*/
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);

		/*----------------SLEEPING-----------------*/

		gettimeofday(&tv, NULL);
		time_in_ms = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ;
		printf("%lld %d is sleeping\n", (long long)time_in_ms, philo->name);
		usleep(philo->time_to_sleep);

		/*----------------END OF SLEEPING-----------------*/


		/*----------------THINKING-----------------*/

		gettimeofday(&tv, NULL);
		time_in_ms = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ;
		printf("%lld %d is thinking\n", time_in_ms, philo->name);

	}
}

void	init_philo(t_philo *philo, t_data *data)
{
	philo->meal_count = 0;
	philo->max_meal_count = data->max_meal_count;
	philo->time_to_eat = data->time_to_eat;
	philo->time_to_sleep = data->time_to_sleep;


	if (philo->name == data->number_of_philosophers - 1)
	{
		philo->left_fork = &(data->philos[philo->name].fork);
		philo->right_fork = &(data->philos[0].fork);
	}
	else 
	{
		philo->left_fork = &(data->philos[philo->name].fork);
		philo->right_fork = &(data->philos[philo->name + 1].fork);
	}
	
	pthread_create(&(philo->thread), NULL, &eating, data->philos + philo->name);
}

void	init_data(t_data *data)
{
	int i = 0;

	while (i < data->number_of_philosophers)
	{
		data->philos[i].name = i;
		pthread_mutex_init(&(data->philos[i].fork), NULL);
		init_philo(&(data->philos[i]), data);
		i++;
	}
	i = 0;
	while (i < data->number_of_philosophers)
	{
		pthread_join(data->philos[i].thread, NULL);
		i++;
	}
}

void	parsing(int argc, char **argv, t_data *data)
{
	t_philo *philos;

	if (!are_arguments_valids(argc, argv))
		exit(EXIT_FAILURE);
	data->number_of_philosophers = ft_atol(argv[1]);
	data->time_to_die = ft_atol(argv[2]);
	data->time_to_eat = ft_atol(argv[3]);
	data->time_to_sleep = ft_atol(argv[4]);
	if (argc == 6)
		data->max_meal_count = ft_atol(argv[5]);
	else
		data->max_meal_count = INFINI;
	philos = malloc(sizeof(t_philo) * data->number_of_philosophers);
	if (!philos)
		exit(EXIT_FAILURE);
	data->philos = philos;

}

int main(int argc, char **argv)
{
	t_data					data;
	
	parsing(argc, argv, &data);
	init_data(&data);
	return (0);
}
