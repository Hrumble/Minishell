/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/09 18:12:46 by ngero             #+#    #+#             */
/*   Updated: 2026/11/09 19:47:04 by ngero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "internal.h"
#include "libft.h"

void	print_vtable()
{
	t_list	*vtable;

	vtable = *get_vtable();
	while (vtable)
	{
		ft_printf("%s=%s\n", *((t_variable *)vtable->content)->key,
				*((t_variable *)vtable->content)->value);
		vtable = vtable->next;
	}
}

int	builtin_env(char **av, int ac, char **env)
{
	(void)av;
	(void)ac;
	if (ac == 2)
		if (ft_strcmp(av[1], "-l"))
			return (print_vtable(), 0);
	while (*env)
		ft_printf("%s\n", *env++);
	return (1);
}
