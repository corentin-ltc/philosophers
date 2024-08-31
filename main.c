#include "philo.h"

int mails = 0;

void *routine(t_data *data)
{
	for (int i = 0; i < 10000000; i++)
	{
		pthread_mutex_lock();
		mails++;
		pthread_mutex_unlock;
	}
}
void	init_philo(pthread_t thread)
{
	pthread_create(thread, NULL, &routine, NULL);
}

void	init_data(t_data *data)
{
	int i = 0;
	while (i < data->number_of_philosophers)
	{
		pthread_mutex_init(&(data->philos[i].fork), NULL);
		init_philo(data->philos[i].thread);
		i++;
	}
}


int main(int argc, char **argv)
{
	pthread_t		t1;
	t_data			data;
	t_philo			*philos;
	
	data.number_of_philosophers = 4;
	philos = malloc(sizeof(t_philo) * data.number_of_philosophers);
	data.philos = philos;

	init_data(&data);
	return 0;
}
