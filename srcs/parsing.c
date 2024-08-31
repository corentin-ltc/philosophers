#include "philo.h"

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

	if (argc < 4 || argc > 5)
	{
		printf("Wrong number of arguments");
		exit(EXIT_FAILURE);
	}
	i = 0;
	while (i < argc)
	{
		if (!str_is_valid_number(args[i]))
			return (false);
	}
}