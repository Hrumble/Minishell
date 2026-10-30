/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/27 18:33:48 by ngero             #+#    #+#             */
/*   Updated: 2026/10/30 12:29:11 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "lexer.h"
#include "libft.h"
#include "debug.h"
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
	if (tk->type == TK_PIPE) ft_printf("Token {type=PIPE}\n");
	if (tk->type == TK_QUOTES) ft_printf("Token {type=QUOTES}\n");
	if (tk->type == TK_BKQUOTES) ft_printf("Token {type=BKQUOTES}\n");
	if (tk->type == TK_BKSLASH) ft_printf("Token {type=BKSLASH}\n");
	if (tk->type == TK_REDIR)
	{
		ft_printf("Token {type=REDIR, ");
		if (tk->redir_type == REDIR_IN)
			ft_printf("redir_type=REDIRIN}\n");
		if (tk->redir_type == REDIR_OUT)
			ft_printf("redir_type=REDIROUT}\n");
		if (tk->redir_type == REDIR_APPENDOUT)
			ft_printf("redir_type=APPENDIN}\n");
		if (tk->redir_type == REDIR_HEREDOC)
			ft_printf("redir_type=HEREDOC}\n");
	}
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

void	print_redir(t_prgrmredir *redir)
{
	if (!redir)
		return ((void)ft_printf("(NULL_REDIR)"));
	ft_printf("redir(type=");
	tostr__t_redirtype(redir->type);
	ft_printf(", target=");
	print_buffer(redir->target, ft_strlen(redir->target));
	ft_printf(")");
}

void	print_ast(t_ast *ast)
{
	int	i;

	i = 0;
	if (!ast)
		return ((void)ft_printf("(NULL_AST)\n"));
	ft_printf("AST {");
	if (ast->type == AST_PRGRM)
	{
		ft_printf("type=PRGRM, argv=");
		print_arr(ast->s_astprgrm.argv, ast->s_astprgrm.ac);
		ft_printf(", ac=%d,\nredir={", ast->s_astprgrm.ac);
		while (i < ast->s_astprgrm.redir_size)
		{
			ft_printf("\t");
			print_redir(ast->s_astprgrm.redir[i++]);
			ft_printf(",\n");
		}
		ft_printf("}, redir_size=%d", ast->s_astprgrm.redir_size);
	}
	ft_printf("}\n");
}
