/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <ngero@student.codam.nl>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 11:38:24 by user              #+#    #+#             */
/*   Updated: 2026/10/30 11:38:32 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEBUG_H
# define DEBUG_H
#include "lexer.h"
#include "parser.h"

// Debug
void	print_buffer(char *buf, size_t buf_size);
void	print_token(t_token *tk);
void	print_ast(t_ast *ast);

// tostr
void	tostr__t_redirtype(t_redirtype type);
void	tostr__t_tokentype(t_tokentype type);

#endif
