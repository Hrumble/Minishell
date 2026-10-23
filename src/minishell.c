/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/23 14:55:42 by ngero             #+#    #+#             */
/*   Updated: 2026/10/23 14:55:49 by ngero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "get_next_line.h"
#include "libft.h"
#include <unistd.h>

static char	*get_user_input()
{
	ft_putstr_fd("penis> ", 1);
	return (get_next_line(STDIN_FILENO));
}

void	minishell()
{
	char	*line;

	line = get_user_input();
	while(line)
	{
		ft_putstr_fd(line, 1);
		free(line);
		line = get_user_input();
	}
}
