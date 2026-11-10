/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/29 20:44:34 by ngero             #+#    #+#             */
/*   Updated: 2026/11/10 11:26:49 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		return (free_ast(&prgrm), NULL);
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
	free_pandlattr(&pattr, &lattr);
	return (l_prgrm);
}

t_ast	*parse_ast(char *buf)
{
	t_parserattr	*pattr;
	t_lexerattr		*lattr;

	pattr = new_parserattr();
	lattr = new_lexerattr(buf);
	if (!pattr || !lattr || !consume_token(pattr, lattr))
		return (free(pattr), free(lattr), NULL);
	return (parse_pipeline(pattr, lattr));
}
