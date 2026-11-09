/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <ngero@student.codam.nl>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/09 10:12:53 by user              #+#    #+#             */
/*   Updated: 2026/11/09 19:13:12 by ngero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "internal.h"
#include "libft.h"
#include <stddef.h>
#include <stdlib.h>

static int	_split_len(char **split)
{
	int	i;

	if (!split || !*split)
		return (0);
	i = 0;
	while (*split)
	{
		i++;
		split++;
	}
	return (i);
}

static int	_parse_var(char *buf)
{
	char	**split;

	split = ft_split(buf, '=');
	if (_split_len(split) != 2)
		return (-1);
	if (!declare_variable(split[0], split[1], 1))
		return (-2);
	free(split);
	return (1);
}

int	builtin_export(char **av, int ac, char **env)
{
	int	out;
	(void)env;
	if (ac < 2)
		return (ft_printf("export: missing variable\n"), 1);
	if (ac > 2)
		return (ft_printf("export: too many args\n"), 1);
	out = _parse_var(av[1]);
	if (out == -1)
		return (ft_printf("export: Incorrect variable format\n"), 1);
	if (out == -2)
		return (ft_printf("export: Failed to declare var\n"), 1);
	return (0);
}
