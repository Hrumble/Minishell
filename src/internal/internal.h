/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internal.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/28 17:44:00 by ngero             #+#    #+#             */
/*   Updated: 2026/10/30 17:11:48 by ngero            ###   ########.fr       */
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

// Gets the user input from the terminal line. 
// line editing included.
int			get_user_input(char *prompt, char *buf);
// Finds the next char that matches the char under `quote_pos`
// return (NULL) if it can't be found
char		*find_matching_char(char *quote_pos);

#endif
