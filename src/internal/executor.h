/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 16:02:26 by ngero             #+#    #+#             */
/*   Updated: 2026/11/05 17:36:20 by ngero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H
# define PATH_SIZE 100

#include "parser.h"

// Executes an ast
int	fork_and_exec(t_ast *ast);

// freeing stuff
void	free_split(char **split);

#endif
