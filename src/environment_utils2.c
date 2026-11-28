/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_utils2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <ngero@student.codam.nl>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/21 23:07:14 by user              #+#    #+#             */
/*   Updated: 2026/11/21 23:09:42 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal.h"
#include "libft.h"
#include <stdlib.h>

t_variable	*seek_variable(char *key)
{
	t_list	*sk;

	sk = *get_vtable();
	while (sk)
	{
		if (!ft_strcmp(key, ((t_variable *)sk->content)->key))
			return (sk->content);
		sk = sk->next;
	}
	return (NULL);
}
