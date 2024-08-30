#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include <stdbool.h> 

typedef struct s_data
{
	int	number_of_philosophers;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	times_eating;
}	t_data;