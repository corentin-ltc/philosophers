#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include <stdbool.h> 
#include <pthread.h>

typedef struct s_data
{
	int	number_of_philosophers;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	times_eating;
	t_philo *philos;

}	t_data;

typedef struct s_philo
{
		int meal_count;
		pthread_mutex_t fork;
		pthread_t thread;
}	t_philo;