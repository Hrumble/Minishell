/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/29 20:44:34 by ngero             #+#    #+#             */
/*   Updated: 2026/11/28 19:12:39 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "arena.h"
#include "lexer.h"
#include "parser.h"
#include <stdlib.h>

static t_ast	*parse_program(t_parserattr *pattr, t_lexerattr *lattr)
{
	t_ast	*prgrm;
	int		ok;

	prgrm = new_astprgrm();
	if (!prgrm)
		return (0);
	while (peek(pattr, TK_WRD) || peek(pattr, TK_REDIR))
	{
		if (peek(pattr, TK_REDIR))
			ok = parse_redir(pattr, lattr, prgrm);
		else if (peek(pattr, TK_WRD))
			ok = parse_word(pattr, lattr, prgrm);
		if (!ok)
			return (free_ast(&prgrm), NULL);
	}
	if (is_empty_prgrm(prgrm))
		return (free(prgrm), NULL);
	return (prgrm);
}

static t_ast	*parse_pipeline(t_parserattr *pattr, t_lexerattr *lattr)
{
	t_ast	*l_prgrm;
	t_ast	*r_prgrm;
	t_ast	*pipe;

	l_prgrm = parse_program(pattr, lattr);
	if (!l_prgrm)
		return (0);
	while (peek(pattr, TK_PIPE))
	{
		if (!consume_token(pattr, lattr))
			return (free_ast(&l_prgrm), NULL);
		r_prgrm = parse_program(pattr, lattr);
		if (!r_prgrm)
			return (free_ast(&l_prgrm), NULL);
		pipe = new_astbinaryop(l_prgrm, r_prgrm);
		if (!pipe)
			return (free_ast(&l_prgrm), free_ast(&r_prgrm), NULL);
		l_prgrm = pipe;
	}
	return (l_prgrm);
}

t_ast	*parse_ast(char *buf)
{
	t_parserattr	*pattr;
	t_lexerattr		*lattr;
	t_arena			*arena;
	t_ast			*ast;

	arena = new_arena(1024);
	if (!arena)
		return (0);
	pattr = new_parserattr(&arena);
	lattr = new_lexerattr(buf, &arena);
	if (!pattr || !lattr || !consume_token(pattr, lattr))
		return (free(arena), NULL);
	ast = parse_pipeline(pattr, lattr);
	free(arena);
	return (ast);
}
