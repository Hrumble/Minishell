/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/26 17:58:51 by ngero             #+#    #+#             */
/*   Updated: 2026/10/30 11:54:08 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "debug.h"
#include "internal.h"
#include "minishell.h"
#include "parser.h"
#include "terminal.h"
#include <stddef.h>
#include <unistd.h>

int	minishell()
{
	t_ast		*ast;
	char		buf[1024];

	terminal_set_icanon(0);
	while (1)
	{
		get_user_input("metasex> ", buf);
		ast = parse_ast(buf);
		ast_map(&ast, (void (*))expand_argv);
		print_ast(ast);
	}

	return (1);
}
