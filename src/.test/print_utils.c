/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/27 18:33:48 by ngero             #+#    #+#             */
/*   Updated: 2026/10/29 23:00:23 by ngero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "lexer.h"
#include "libft.h"
#include "parser.h"
#include <stddef.h>

void	print_buffer(char *buf, size_t buf_size)
{
	size_t	i;

	ft_printf("Buffer(%d): ", buf_size);
	i = 0;
	while (i < buf_size)
	{
		if (ft_isprint(buf[i]))
			ft_putchar(buf[i]);
		else
			ft_printf("0x%x", buf[i]);
		i++;
	}
}

void	print_token(t_token *tk)
{
	if (tk->type == TK_WRD)
	{
		ft_printf("Token {type=%s, v=", "WRD");
		print_buffer(tk->atom.ptr, tk->atom.size);
		ft_printf("}\n");
	}
	if (tk->type == TK_PIPE)
		ft_printf("Token {type=%s}\n", "PIPE");
	if (tk->type == TK_QUOTES)
		ft_printf("Token {type=%s}\n", "QUOTES");
	if (tk->type == TK_BKQUOTES)
		ft_printf("Token {type=%s}\n", "BKQUOTES");
	if (tk->type == TK_BKSLASH)
		ft_printf("Token {type=%s}\n", "BKSLASH");
	if (tk->type == TK_REDIRIN)
		ft_printf("Token {type=%s}\n", "REDIRIN");
	if (tk->type == TK_REDIROUT)
		ft_printf("Token {type=%s}\n", "REDIROUT");
	if (tk->type == TK_HEREDOC)
		ft_printf("Token {type=%s}\n", "HEREDOC");
	if (tk->type == TK_APPENDOUT)
		ft_printf("Token {type=%s}\n", "APPENDOUT");
}

void	print_arr(char **arr, size_t size)
{
	size_t	i;

	i = 0;
	ft_printf("{");
	while (i < size)
		ft_printf("%s, ", arr[i++]);
	ft_printf("}");
}

void	print_ast(t_ast *ast)
{
	if (!ast)
		return ((void)ft_printf("AST (NULL)\n"));
	ft_printf("AST {");
	if (ast->type == AST_PRGRM)
	{
		ft_printf("type=PRGRM, argv=");
		print_arr(ast->s_astprgrm.argv, ast->s_astprgrm.ac);
		ft_printf(", ac=%d", ast->s_astprgrm.ac);
	}
	ft_printf("}\n");
}
