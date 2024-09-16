/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-tort <cle-tort@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 17:09:46 by cle-tort          #+#    #+#             */
/*   Updated: 2024/09/16 17:12:01 by cle-tort         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
# define TEXT 0
# define CHECK 1
# define DEATH 2
# define THINK 3
# define READY 4

typedef struct s_philo
{
	int				name;
	int				meal_count;
	bool			is_thinking;
	long long		last_meal;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_t		thread;
	long long		start_time;
	struct s_data	*data;
}	t_philo;

typedef struct s_data
{
	int				nbr_of_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				max_meal_count;
	bool			someone_is_dead;
	pthread_t		monitor_thread;
	pthread_mutex_t	*mutex;
	pthread_mutex_t	*forks;
	t_philo			*philos;
}	t_data;

bool		are_arguments_valids(int argc, char **args);
long		ft_atol(char *str);
bool		is_starved(t_philo *philo, bool just_checking);
bool		get_bool(pthread_mutex_t *mutex, bool *boolean);
long long	timenow(t_philo *philo);
void		ft_usleep(long long microseconds);
void		m_printf(char *message, long long time,
				t_philo philo, bool is_death);
bool		is_starved(t_philo *philo, bool just_checking);
void		monitor(t_data *data);
int			one_philo(t_data *data);
int			start_threads(t_data *data, long long time_in_ms);
void		routine(t_philo *philo);
int			init_data(t_data *data);
int			parsing(int argc, char **argv, t_data *data);
bool		get_bool(pthread_mutex_t *mutex, bool *boolean);
size_t		ft_strlen(char *str);

#endif
