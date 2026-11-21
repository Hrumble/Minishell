/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/26 17:58:51 by ngero             #+#    #+#             */
/*   Updated: 2026/11/21 21:20:34 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "internal.h"
#include "minishell.h"
#include "parser.h"
#include <stddef.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

static t_ast	*_parse_user_input(char *input)
{
	t_ast	*ast;

	ast = parse_ast(input);
	if (!ast)
		return (0);
	if (!ast_map(&ast, expand_argv)
		|| !ast_map(&ast, expand_redir))
		free_ast(&ast);
	return (ast);
}

static void	_cleanup(t_ast **ast, char **env, char **buf)
{
	free_ast(ast);
	free_split(env);
	free(*buf);
	*buf = 0;
}

static void _read_loop(void)
{
	t_ast	*ast;
	char	*buf;
	char	**varenv;

	buf = readline("\033[33mmetasex>\033[0m ");
	if (!buf || !*buf)
		return (free(buf));
	add_history(buf);
	ast = _parse_user_input(buf);
	if (!ast)
		return (ft_putstr_fd("metasex: parse error\n", 2));
	varenv = get_env();
	if (!varenv)
		return (_cleanup(&ast, varenv, &buf));
	execute_pipeline(ast, varenv);
	_cleanup(&ast, varenv, &buf);
}

int	minishell(char **env)
{
	if (!init_vtable(env))
		return (0);
	using_history();
	while (1)
		_read_loop();
	clear_history();
	free_vtable();
	return (1);
}
