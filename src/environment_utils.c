/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <ngero@student.codam.nl>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/09 11:19:42 by user              #+#    #+#             */
/*   Updated: 2026/11/10 10:05:13 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "internal.h"
#include "libft.h"
#include <stddef.h>
#include <stdlib.h>

t_list	**init_vtable(char **env)
{
	char	**split;
	t_list	**vtable;

	vtable = get_vtable();
	while (*env)
	{
		split = ft_split(*env, '=');
		if (!split)
			return (0);
		if (!declare_variable(split[0], split[1], 1))
			return (free_split(split), (void*)0);
		free(split);
		env++;
	}
	return (vtable);
}

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

char	*tostr__variable(t_variable *var)
{
	size_t	total_size;
	size_t	vallen;
	char	*str;

	if (!var)
		return (0);
	vallen = 0;
	if (var->value)
		vallen = ft_strlen(var->value);
	total_size = ft_strlen(var->key) + vallen + 1;
	str = malloc((total_size + 1) * sizeof(char));
	if (!str)
		return (0);
	ft_strlcpy(str, var->key, total_size + 1);
	str[ft_strlen(var->key)] = '=';
	str[ft_strlen(var->key) + 1] = 0;
	if (vallen)
		ft_strlcat(str, var->value, total_size + 1);
	return (str);
}

void	free_variable(t_variable *var)
{
	if (!var)
		return ;
	free(var->key);
	free(var->value);
	free(var);
}

void	free_vtable()
{
	t_list	**vtable;

	vtable = get_vtable();
	ft_lstclear(vtable, (void (*)(void *))free_variable);
}
