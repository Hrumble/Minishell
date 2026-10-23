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

#include <unistd.h>
#include "libft.h"
#include "printf.h"
#include "get_next_line.h"

typedef struct s_sstatus {
} t_sstatus;

// Launches the minishell process
void	minishell();

#endif
