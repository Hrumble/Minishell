/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 17:53:17 by ngero             #+#    #+#             */
/*   Updated: 2026/11/06 17:23:55 by ngero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include <stdlib.h>
#include <unistd.h>

int	builtin_cd(char **av, int ac, char **env)
{
	char	*home;

	if (ac < 2)
	{
		home = getenv("HOME");
		if (!home)
			return (1);
		return (chdir(home));
	}
	return (0);
}

