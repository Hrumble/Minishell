/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <ngero@student.codam.nl>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/09 10:47:48 by user              #+#    #+#             */
/*   Updated: 2026/11/09 11:14:36 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal.h"
#include "libft.h"
#include <stdlib.h>

t_list	**get_vartable()
{
	static t_list	*vartable = 0;

	return (&vartable);
}

int		add_variable(char *key, char *val, int isexported)
{
	t_list		*new;
	t_list		**vartable;
	t_variable	*var;

	vartable = get_vartable();
	var = malloc(sizeof(t_variable));
	if (!var)
		return (0);
	var->key = key;
	var->value = val;
	var->exported = isexported;
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
	vartable = get_vartable();
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
