/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <ngero@student.codam.nl>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/09 10:12:53 by user              #+#    #+#             */
/*   Updated: 2026/11/10 10:33:11 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "internal.h"
#include "libft.h"
#include <stddef.h>
#include <stdlib.h>

static int	_declare_or_free(char *key, char *val)
{
	if (!declare_variable(key, val, 1))
		return (free(key), free(val), 0);
	return (1);
}

static int	_parse_var(char *buf)
{
	int		len[2];
	char	*key;

	if (!buf || *buf == '=')
		return (0);
	len[0] = len[1] = 0;
	while (buf[len[0]] && buf[len[0]] != '=')
		len[0]++;
	key = ft_substr(buf, 0, len[0]);
	if (!buf[len[0]])
		return _declare_or_free(key, NULL);
	if (!buf[len[0] + 1])
		return _declare_or_free(key, ft_strdup(""));
	while (buf[len[0] + 1 + len[1]])
		len[1]++;
	return (_declare_or_free(key, ft_substr(buf, len[0] + 1, len[1])));
}

int	builtin_export(char **av, int ac, char **env)
{
	(void)env;
	if (ac < 2)
		return (ft_printf("export: missing variable\n"), 1);
	if (ac > 2)
		return (ft_printf("export: too many args\n"), 1);
	if (!_parse_var(av[1]))
		return (ft_printf("Failed to define variable\n"), 1);
	return (0);
}
