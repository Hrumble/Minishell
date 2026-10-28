#include "builtins.h"
#include "ft_printf.h"
#include "libft.h"

int	builtin_echo(char **av, int ac)
{
	int	n_flag;
	int	i;

	if (ac < 1)
		return (0);
	n_flag = 0;
	i = 1;
	if (!ft_strcmp(av[1], "-n"))
	{
		n_flag = 1;
		i++;
	}
	while (i < ac)
		ft_printf("%s ", av[i++]);
	if (!n_flag)
		ft_putchar('\n');
	return (1);
}
