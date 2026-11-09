/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <ngero@student.codam.nl>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/09 10:47:48 by user              #+#    #+#             */
/*   Updated: 2026/11/09 11:21:46 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "internal.h"
#include "libft.h"
#include <stddef.h>
#include <stdlib.h>

t_list	**get_vtable()
{
	static t_list	*vartable = 0;

	return (&vartable);
}

int		declare_variable(char *key, char *val, int is_exported)
{
	t_list		*new;
	t_list		**vartable;
	t_variable	*var;

	if (!key)
		return (0);
	if (!val)
		val = ft_strdup("");
	vartable = get_vtable();
	var = new_variable(key, val, is_exported);
	if (!var)
		return (0);
	new = ft_lstnew(var);
	if (!new)
		return (free(var), 0);
	ft_lstadd_back(vartable, new);
	return (1);
}

t_variable	*remove_variable(char *key)
{
	t_list		*sk;
	int			index;
	t_variable	*var;
	t_list		**vartable;

	if (!key)
		return (0);
	vartable = get_vtable();
	sk = *vartable;
	index = 0;
	while (sk && ft_strcmp(((t_variable *)sk->content)->key, key))
	{
		sk = sk->next;
		index++;
	}
	if (!sk)
		return (0);
	var = (t_variable *)sk->content;
	ft_lstremove(vartable, index, 0);
	return (var);
}

char	**get_env()
{
	t_list	*vartable;
	size_t	i;
	size_t	lstsize;
	char	**env;

	vartable = *get_vtable();
	lstsize = ft_lstsize(vartable);
	env = malloc((lstsize + 1) * sizeof(char *));
	if (!env)
		return (0);
	i = 0;
	while (vartable)
	{
		env[i] = tostr__variable(vartable->content);
		if (!env[i])
			return (free_split(env), (void *)0);
		vartable = vartable->next;
		i++;
	}
	env[i] = 0;
	return (env);
}
