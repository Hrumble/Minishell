/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internal.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/28 17:44:00 by ngero             #+#    #+#             */
/*   Updated: 2026/10/28 17:44:48 by ngero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERNAL_H
# define INTERNAL_H
# define CMDLINE_BUFSIZE 10
# include <stddef.h>

typedef struct s_lineinput {
	char	*buffer;
	char	*buf_ptr;
	size_t	buf_size;
} t_lineinput;

typedef struct s_cmdline {
	char	**av;
	int		ac;
} t_cmdline;

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
void	print_cmdline(t_cmdline *cmdline);

// utils
// Counts the amount of words in a null terminated array of (?)
int			count_split(const void **sp);
// frees a null terminated array of (?)
void		*free_split(char **av);
// Frees a t_cmdline struct
void		free_cmdline(t_cmdline *cmdline);

#endif
