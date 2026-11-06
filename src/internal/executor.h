/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 16:02:26 by ngero             #+#    #+#             */
/*   Updated: 2026/11/05 17:48:39 by ngero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H
# define PATH_SIZE 100

#include "parser.h"

// Executes an ast
int	fork_and_exec(t_ast *ast, char **env);

// Finds a binary from relative path or PATH
// returns (void *)0 on failure to find.
char	*find_binary(char *buf);
// Runs a builtin function, providing it **av and ac. Sets the status.
// always returns (1)
int		run_builtin(int (*builtin)(char **, int), char **av, int ac);

// freeing stuff
void	free_split(char **split);

#endif
