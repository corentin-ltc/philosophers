#ifndef PHILO_H
# define PHILO_H


# include <stdio.h> 
# include <unistd.h> 
# include <stdlib.h> 
# include <stdbool.h> 
# include <pthread.h>
# include <sys/time.h>
# include <time.h>
# include <limits.h>

# define INFINI -1

typedef struct s_monitor
{
	bool	a_philo_starved_to_death;
 }	t_monitor;

typedef struct s_philo
{
	int				number_of_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				max_meal_count;
	int				name;
	int				meal_count;
	long long		time_of_last_meal;
	pthread_mutex_t fork;
	pthread_mutex_t *death;
	pthread_mutex_t *left_fork;
	pthread_mutex_t *right_fork;
	pthread_t 		thread;
	t_monitor		*monitor;

}	t_philo;

typedef struct s_data
{
	int	number_of_philosophers;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	max_meal_count;

	pthread_mutex_t *death;
	t_philo *philos;
}	t_data;

bool	are_arguments_valids(int argc, char **args);
long	ft_atol(char *str);

#endif
