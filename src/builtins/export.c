/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <ngero@student.codam.nl>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/09 10:12:53 by user              #+#    #+#             */
/*   Updated: 2026/11/09 10:40:49 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"
#include <stddef.h>

static char	*_parse_var(char *buf)
{
	char	*sk;

	sk = buf;
	if (*sk == '=')
		return (0);
	while (*sk && *sk != '=')
		sk++;
	if (*sk != '=')
		return (0);
	sk++;
	if (!*sk)
		return (0);
	while (*sk)
		sk++;
	return (ft_strdup(buf));
}

static int	_append_to_env(char **env)
{
	char	**sk;

	sk = env;
	while (*sk)
		(*sk)++;
}

int	builtin_export(char **av, int ac, char **env)
{
	if (ac < 2)
		return (ft_printf("export: missing variable\n"), 1);
	if (ac > 2)
		return (ft_printf("export: too many args\n"), 1);
}
