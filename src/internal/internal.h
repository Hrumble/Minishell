/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internal.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/28 17:44:00 by ngero             #+#    #+#             */
/*   Updated: 2026/11/09 11:21:13 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERNAL_H
# define INTERNAL_H
#include "libft.h"
# include "parser.h"
# define CMDLINE_BUFSIZE 10
# define EXPANSION_SIZE 100
# include <stddef.h>

typedef struct s_lineinput
{
	char	*buffer;
	char	*buf_ptr;
	size_t	buf_size;
}	t_lineinput;

typedef struct s_bufexpand
{
	size_t	exsize;
	char	*expanded;
	char	*buf;
	int		in_squotes;
}	t_bufexpand;

typedef struct s_variable
{
	char	*key;
	char	*value;
	int		exported;
}	t_variable;
// Gets the user input from the terminal line. 
// line editing included.
int			get_user_input(char *prompt, char *buf);
// Finds the next char that matches the char under `quote_pos`
// return (NULL) if it can't be found
char		*find_matching_char(char *quote_pos);
// Expands the program arguments
int			expand_argv(t_ast	**prgrm);
// Expands all the redirections of a program
int			expand_redir(t_ast	**prgrm);
// Resizes the expansion char
int			resize_expansion(t_bufexpand *ex, size_t env_size, size_t i);
// Copies the expanded environment var into the expanded char *
int			env_expansioncpy(t_bufexpand *ex, char *env, size_t *i);
// sets up program expansion
t_ast		*setup_expansion(t_ast **prgrm);
// sets up buffer expansion
int			setup_buf_expansion(t_bufexpand *ex, size_t *i, char *buf);

// Environment

// returns the vartable.
t_list		**get_vtable();
// frees the vartable
void		free_vtable();
// Adds the variable with the provided key and value to the vartable
// returns 0 on failure
int			declare_variable(char *key, char *val, int isexported);
// removes the variable with key `key` from the vartable.
// returns the variable to be freed or 0 if it was not found
t_variable	*remove_variable(char *key);
// returns the environment null terminated array built from the vartable
char		**get_env();
// Creates a new t_variable struct with the provided key val pair
t_variable	*new_variable(char *key, char *val, int is_exported);
// Frees a variable
void		free_variable(t_variable *var);
// converts a variable to string
char		*tostr__variable(t_variable *var);
// initializes the variable table with the provided env
t_list		**init_vtable(char **env);

// Utils
// Copies a string array
char		**ft_strarrcpy(char **dest, char **arr, size_t size);

#endif
