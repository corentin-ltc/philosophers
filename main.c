#include "philo.h"

void eating(t_philo *philo)
{
	struct timeval			tv;
	long long					time_in_ms;
	
	while (philo->meal_count < philo->max_meal_count)
	{
		pthread_mutex_lock(philo->left_fork);
		gettimeofday(&tv, NULL);
		time_in_ms = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ;
		printf("%lld %d has taken a fork\n", (long long)time_in_ms, philo->name);	
		pthread_mutex_lock(philo->right_fork);
		gettimeofday(&tv, NULL);
		time_in_ms = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ;
		printf("%lld %d has taken a fork\n", (long long)time_in_ms, philo->name);
		
		/*----------------EATING-----------------*/

		gettimeofday(&tv, NULL);
		time_in_ms = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ;
		printf("%lld %d is eating\n", (long long)time_in_ms, philo->name);
		usleep(philo->time_to_eat);
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
	philo->max_meal_count = 4;
	philo->time_to_eat= 100000;
	philo->time_to_sleep= 100000;


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

void	parsing(t_data *data, t_philo *philo)
{
	
	data->philos = malloc(sizeof(t_philo) * data->number_of_philosophers);
	if (!data->philos)
		exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
	t_data					data;
	t_philo					*philos;
	

	init_data(&data);
	return (0);
}
