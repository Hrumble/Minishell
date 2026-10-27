/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/23 14:52:59 by ngero             #+#    #+#             */
/*   Updated: 2026/10/23 14:56:42 by ngero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stddef.h>
#include <unistd.h>
#include "libft.h"
#include "printf.h"
#include "get_next_line.h"

typedef struct s_lineinput {
	char	*buffer;
	char	*buf_ptr;
	size_t	buf_size;
} t_lineinput;

// Launches the minishell process
int	minishell();

#endif
