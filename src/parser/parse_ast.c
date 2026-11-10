/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/29 20:44:34 by ngero             #+#    #+#             */
/*   Updated: 2026/11/10 10:36:05 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft.h"
#include "parser.h"
#include <stdlib.h>

inline static t_ast	*parse_definition(t_parserattr *pattr, t_lexerattr *lattr)
{
	t_ast	*new;

	if (peek(pattr, TK_DEF))
	{
		new = new_astdef(&pattr->current->defvar);
		consume_token(pattr, lattr);
		return (new);
	}
	return (0);
}

static t_ast	*parse_program(t_parserattr *pattr, t_lexerattr *lattr)
{
	char	*substr;
	int		ps_redir_out;
	t_ast	*prgrm;

	prgrm = new_astprgrm();
	if (!prgrm)
		return (0);
	while (peek(pattr, TK_WRD) || peek(pattr, TK_REDIR))
	{
		ps_redir_out = parse_redir(pattr, lattr, prgrm);
		if (ps_redir_out == -1)
			return (free_prgrm(&prgrm), (void *)0);
		else if (ps_redir_out && consume_token(pattr, lattr))
			continue ;
		substr = ft_substr(pattr->current->atom.ptr, 0,
				pattr->current->atom.size);
		if (!substr)
			return (free_prgrm(&prgrm), (void *)0);
		prgrm->s_prgrm.argv[prgrm->s_prgrm.ac] = substr;
		prgrm->s_prgrm.ac++;
		consume_token(pattr, lattr);
	}
	if (is_empty_prgrm(prgrm))
		return (free_prgrm(&prgrm), (void *)0);
	return (prgrm);
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
	if (!l_prgrm)
		return (0);
	while (peek(pattr, TK_PIPE))
	{
		if (!consume_token(pattr, lattr))
			return (free_prgrm(&l_prgrm), (void *)0);
		r_prgrm = parse_program(pattr, lattr);
		if (!r_prgrm)
			return (free_prgrm(&l_prgrm), (void *)0);
		pipe = new_astbinaryop(l_prgrm, r_prgrm);
		if (!pipe)
			return (free_prgrm(&l_prgrm), free_prgrm(&r_prgrm), (void *)0);
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
