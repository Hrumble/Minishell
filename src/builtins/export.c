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
#include "lexer.h"
#include "libft.h"
#include <stddef.h>
#include <stdlib.h>

static int	_declare_or_free(char *key, char *val)
{
	if (!key)
		return (0);
	if (!declare_variable(key, val, 1))
		return (free(key), free(val), 0);
	return (1);
}

static char	*_parse_key(char *buf)
{
	char	*start;
	char	*end;

	start = end = buf;
	if (ft_isdigit(*end))
		return (NULL);
	while(ft_isvar(*end))
		end++;
	if (*end && *end != '=')
		return (NULL);
	return (ft_substr(buf, 0, end - start));
}

static char	*_parse_val(char *buf)
{
	char	*start;
	char	*end;

	start = end = buf;
	while (ft_isvar(*end))
		end++;
	if (*end++ != '=')
		return (NULL);
	start = end;
	while (*end)
		end++;
	return (ft_substr(start, 0, end - start));
}

static int	_parse_var(char *buf)
{
	char	*key;
	char	*val;

	if (!buf || *buf == '=')
		return (0);
	key = _parse_key(buf);
	val = _parse_val(buf);
	return (_declare_or_free(key, val));
}

int	builtin_export(char **av, int ac, char **env)
{
	(void)env;
	if (ac < 2)
		return (ft_putstr_fd("export: missing variable\n", 2), 1);
	if (!_parse_var(av[1]))
		return (ft_putstr_fd("not a valid identifier\n", 2), 1);
	return (0);
}
