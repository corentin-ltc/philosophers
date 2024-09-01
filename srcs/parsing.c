#include "philo.h"

size_t	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

bool	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (true);
	else
		return (false);
}

void	exit_error(char *msg)
{
	//free everything
	write(STDERR_FILENO, msg, ft_strlen(msg));
	exit(EXIT_FAILURE);
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
			exit_error("Number of milliseconds can't be negative.");
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
			exit_error("Number of milliseconds is too high.");
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
			break;
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
		printf("Wrong number of arguments");
		return (false);
	}
	i = 1;
	while (i < argc)
	{
		if (!str_is_valid_number(args[i]))
		{
			printf("%s is not a valid number", args[i]);
			return (false);
		}
		i++;
	}
	return (true);
}