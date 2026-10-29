/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tester.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/29 17:27:47 by ngero             #+#    #+#             */
/*   Updated: 2026/10/29 22:48:25 by ngero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TESTER_H
# define TESTER_H
#include "lexer.h"
#include "parser.h"

// Debug
void	print_buffer(char *buf, size_t buf_size);
void	print_token(t_token *tk);
void	print_ast(t_ast *ast);

#endif
