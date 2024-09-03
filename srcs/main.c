#include "philo.h"

long long	timenow(void)
{
	struct timeval			tv;
	long long					time_in_ms;

	gettimeofday(&tv, NULL);
	time_in_ms = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ;
	return (time_in_ms);
}

bool	is_starved(t_philo *philo, bool just_checking)
{
	if (philo->monitor->a_philo_starved_to_death)
		return (true);
	if (!just_checking && timenow() - philo->time_of_last_meal >= philo->time_to_die)
	{
		pthread_mutex_lock(philo->death);
		printf("%lld %d died\n", philo->time_of_last_meal + philo->time_to_die, philo->name);
		philo->monitor->a_philo_starved_to_death = true;
		return (true);
	}
	return (false);
}


void routine(t_philo *philo)
{
	while (!is_starved(philo, true) && (philo->max_meal_count == INFINI || philo->meal_count < philo->max_meal_count))
	{
		if (philo->name % 2 == 0)
		{
			pthread_mutex_lock(philo->left_fork);
			if (is_starved(philo, false))
				break;
			printf("%lld %d has taken a fork\n", timenow(), philo->name);	
			pthread_mutex_lock(philo->right_fork);
			if (is_starved(philo, false))
				break;
			printf("%lld %d has taken a fork\n", timenow(), philo->name);
		}
		else 
		{
			pthread_mutex_lock(philo->right_fork);
			if (is_starved(philo, false))
				break;
			printf("%lld %d has taken a fork\n", timenow(), philo->name);
			pthread_mutex_lock(philo->left_fork);
			if (is_starved(philo, false))
				break;
			printf("%lld %d has taken a fork\n", timenow(), philo->name);
		}
		
		/*----------------EATING-----------------*/
		printf("%lld %d is eating\n", timenow(), philo->name);
		philo->time_of_last_meal = timenow();
		if (philo->time_to_eat >= philo->time_to_die)
		{
			usleep(philo->time_to_die * 1000);
			pthread_mutex_lock(philo->death);
			printf("%lld %d died\n", timenow(), philo->name);
			philo->monitor->a_philo_starved_to_death = true;
			break;
		}
		usleep(philo->time_to_eat * 1000);
		if (philo->max_meal_count != INFINI)
			philo->meal_count++;
		
		/*----------------END OF EATING-----------------*/
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		/*----------------SLEEPING-----------------*/
		if (is_starved(philo, true))
			break;
		printf("%lld %d is sleeping\n", timenow(), philo->name);
		if (timenow() - philo->time_of_last_meal + philo->time_to_sleep >= philo->time_to_die)
		{
			philo->monitor->a_philo_starved_to_death = true;
			usleep(philo->time_to_die - (timenow() - philo->time_of_last_meal));
			pthread_mutex_lock(philo->death);
			printf("%lld %d died\n", timenow(), philo->name);
			break;
		}
		usleep(philo->time_to_sleep * 1000);
		/*----------------END OF SLEEPING-----------------*/
		/*----------------THINKING-----------------*/
		if (is_starved(philo, true))
			break;
		printf("%lld %d is thinking\n", timenow(), philo->name);
	}
	
}

void	init_philo(t_philo *philo, t_data *data, t_monitor *monitor)
{
	philo->monitor = monitor;
	monitor->a_philo_starved_to_death = false;
	philo->time_of_last_meal = timenow();
	philo->meal_count = 0;
	philo->max_meal_count = data->max_meal_count;
	philo->time_to_eat = data->time_to_eat;
	philo->time_to_sleep = data->time_to_sleep;
	philo->time_to_die = data->time_to_die;
	philo->death = data->death;
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
	pthread_create(&(philo->thread), NULL, &routine, data->philos + philo->name);
}

void	init_data(t_data *data)
{
	int				i;
	t_monitor		monitor;

	if (data->number_of_philosophers == 1)
	{		
		printf("%lld %d has taken a fork\n", timenow(), 0);
		usleep(data->time_to_die * 1000);
		printf("%lld %d died\n", timenow(), 0);
		return ;
	}
	pthread_mutex_init(data->death, NULL);
	i = 0;
	while (i < data->number_of_philosophers)
	{
		data->philos[i].name = i;
		pthread_mutex_init(&(data->philos[i].fork), NULL);
		init_philo(&(data->philos[i]), data, &monitor);
		i++;
	}
	i = 0;
	while (i < data->number_of_philosophers)
	{
		pthread_join(data->philos[i].thread, NULL);
		i++;
	}
}

int	parsing(int argc, char **argv, t_data *data)
{
	t_philo *philos;

	if (!are_arguments_valids(argc, argv))
		return (0);
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
		return (0);
	data->philos = philos;
	return (1);
}

int main(int argc, char **argv)
{
	t_data					data;
	
	if (!parsing(argc, argv, &data))
		return (1);
	init_data(&data);
	return (0);
}
