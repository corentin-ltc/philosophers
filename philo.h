#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include <stdbool.h> 
#include <pthread.h>
#include <sys/time.h>
#include <time.h>
#include <limits.h>


typedef struct s_philo
{
	int	number_of_philosophers;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	max_meal_count;
	int name;
	int meal_count;
	pthread_mutex_t fork;
	pthread_mutex_t *left_fork;
	pthread_mutex_t *right_fork;
	pthread_t thread;
}	t_philo;

typedef struct s_data
{
	int	number_of_philosophers;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	max_meal_count;


	t_philo *philos;
}	t_data;

