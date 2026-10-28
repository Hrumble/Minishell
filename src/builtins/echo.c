#include "builtins.h"
#include "ft_printf.h"

int	builtin_echo(char **av, int ac)
{
	int	i;

	if (ac < 1)
		return (0);
	i = 1;
	while (i < ac)
		ft_putstr(av[i++]);
	ft_putchar('\n');

	return (1);
}
