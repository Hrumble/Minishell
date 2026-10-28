/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/23 14:52:59 by ngero             #+#    #+#             */
/*   Updated: 2026/10/27 20:59:30 by ngero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stddef.h>
#include <unistd.h>

typedef struct s_lineinput {
	char	*buffer;
	char	*buf_ptr;
	size_t	buf_size;
} t_lineinput;

typedef struct s_cmdline {
	char	**av;
	int		ac;
} t_cmdline;

// Launches the minishell process
int			minishell();
// Gets the user input from the terminal line. 
// line editing included.
int			get_user_input(char *prompt, char *buf);
// Parses a command line
t_cmdline	*parse_command_line(const char *c);
// Interprets a command
int			interpret_command_line(t_cmdline *cmdline);
// Splits a command line into its av equivalent
char		**split_cmdline(const char *c);

// Debug
void	print_buffer(char *buf, size_t buf_size);

// utils
// Counts the amount of words in a null terminated array of (?)
int			count_split(const void **sp);
// frees a null terminated array of (?)
void		*free_split(char **av);
// Frees a t_cmdline struct
void		free_cmdline(t_cmdline *cmdline);

#endif
