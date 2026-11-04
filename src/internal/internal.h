/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internal.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/28 17:44:00 by ngero             #+#    #+#             */
/*   Updated: 2026/11/01 22:20:25 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERNAL_H
# define INTERNAL_H
# include "parser.h"
# define CMDLINE_BUFSIZE 10
# define EXPANSION_SIZE 100
# include <stddef.h>

typedef struct s_lineinput {
	char	*buffer;
	char	*buf_ptr;
	size_t	buf_size;
} t_lineinput;

// Gets the user input from the terminal line. 
// line editing included.
int			get_user_input(char *prompt, char *buf);
// Finds the next char that matches the char under `quote_pos`
// return (NULL) if it can't be found
char		*find_matching_char(char *quote_pos);
// Expands the program arguments
int			expand_argv(t_ast	**prgrm);

// Utils
// Copies a string array
char		**ft_strarrcpy(char **dest, char **arr, size_t size);
// Free an array of pointers (frees the pointers too)
void		free_arr(void **arr, size_t size);

#endif
