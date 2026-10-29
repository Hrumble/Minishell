/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internal.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/28 17:44:00 by ngero             #+#    #+#             */
/*   Updated: 2026/10/29 17:58:09 by ngero            ###   ########.fr       */
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

#endif
