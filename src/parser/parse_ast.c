/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/29 20:44:34 by ngero             #+#    #+#             */
/*   Updated: 2026/11/03 18:01:02 by ngero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "parser.h"
#include <stdlib.h>

static t_ast	*parse_program(t_parserattr *pattr, t_lexerattr *lattr)
{
	return (0);
}

// && grammar:
// (command or pipe) (&& command)?

// | grammar:
// (command or pipe) (| command)?
static t_ast	*parse_pipeline(t_parserattr *pattr, t_lexerattr *lattr)
{
	t_ast	*l_prgrm;
	t_ast	*r_prgrm;
	t_ast	*pipe;

	l_prgrm = parse_program(pattr, lattr);
	while (peek(pattr, TK_PIPE))
	{
		if (!consume_token(pattr, lattr))
			return (free_prgrm(l_prgrm), (void *)0);
		r_prgrm = parse_program(pattr, lattr);
		if (!r_prgrm)
			return (free_prgrm(l_prgrm), (void *)0);
		pipe = new_astbinaryop(l_prgrm, r_prgrm);
		if (!pipe)
			return (free_prgrm(l_prgrm), free_prgrm(r_prgrm), (void *)0);
		l_prgrm = pipe;
	}
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
