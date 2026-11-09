/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <ngero@student.codam.nl>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/09 11:19:42 by user              #+#    #+#             */
/*   Updated: 2026/11/09 11:24:22 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal.h"
#include "libft.h"
#include <stddef.h>
#include <stdlib.h>

t_variable	*new_variable(char *key, char *val, int is_exported)
{
	t_variable	*var;

	var = malloc(sizeof(t_variable));
	if (!var)
		return (0);
	var->key = key;
	var->value = val;
	var->exported = is_exported;
	return (var);
}

char	*variable__tostr(t_variable *var)
{
	size_t	total_size;
	char	*str;

	if (!var)
		return (0);
	total_size = ft_strlen(var->key) + ft_strlen(var->value) + 1;
	str = malloc((total_size + 1) * sizeof(char));
	if (!str)
		return (0);
}
