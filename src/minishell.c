/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/26 17:58:51 by ngero             #+#    #+#             */
/*   Updated: 2026/11/08 18:52:27 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "internal.h"
#include "minishell.h"
#include "parser.h"
#include "ft_printf.h"
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>

static t_ast	*parse_user_input(char *input)
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

static void	cleanup(t_ast **ast, char **env, char **buf)
{
	free_ast(ast);
	free_split(env);
	free(*buf);
	*buf = 0;
}

int	minishell(char **env)
{
	t_ast	*ast;
	char	*buf;
	char	**varenv;

	if (!init_vtable(env))
		return (0);
	// terminal_set_icanon(0);
	while (1)
	{
		varenv = 0;
		buf = readline("\033[33mmetasex>\033[0m ");
		if (!buf)
			return (0);
		ast = parse_user_input(buf);
		if (!ast)
			ft_putstr("\033[31mParse error\033[0m\n");
		else
		{
			varenv = get_env();
			if (!varenv)
				return (cleanup(&ast, NULL, &buf), 0);
			execute_pipeline(ast, varenv);
		}
		cleanup(&ast, varenv, &buf);
	}
	free_vtable();
	return (1);
}
