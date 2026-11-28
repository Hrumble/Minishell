/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/28 17:47:04 by ngero             #+#    #+#             */
/*   Updated: 2026/11/28 18:48:37 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include "lexer.h"

static t_token	*get_single_char_token(t_lexerattr *attr)
{
	if (*attr->ptr == '|')
		return (new_token(TK_PIPE, 0, 0, attr->arena));
	if (*attr->ptr == '<')
	{
		if (*(attr->ptr + 1) == '<')
			return (attr->ptr++, new_token_redir(REDIR_HEREDOC, attr->arena));
		return (new_token_redir(REDIR_IN, attr->arena));
	}
	if (*attr->ptr == '>')
	{
		if (*(attr->ptr + 1) == '>')
			return (attr->ptr++, new_token_redir(REDIR_APPENDOUT, attr->arena));
		return (new_token_redir(REDIR_OUT, attr->arena));
	}
	return (0);
}

static void	walk_quotes(t_lexerattr *lattr)
{
	char	quote_char;

	quote_char = *lattr->ptr++;
	while (*lattr->ptr != quote_char && *lattr->ptr)
		lattr->ptr++;
}

// hello
// "hello"
// 'hello'
// "hey it's me! 'test'"
// "hello""test"
// he'l'lo
// test='cunt'
// test='test'"hello"
static t_token	*get_wrd_token(t_lexerattr *lattr)
{
	char	*wrd_start;

	wrd_start = 0;
	while (ft_iswrd(*lattr->ptr) || ft_isquotes(*lattr->ptr))
	{
		if (!wrd_start)
			wrd_start = lattr->ptr;
		if (ft_isquotes(*lattr->ptr))
			walk_quotes(lattr);
		if (!*lattr->ptr)
			return (0);
		lattr->ptr++;
	}
	if (wrd_start)
		return (new_token(TK_WRD, wrd_start,
					lattr->ptr - wrd_start, lattr->arena));
	return (0);
}

t_token	*next_token(t_lexerattr *attr)
{
	t_token	*new_token;

	while (*attr->ptr == ' ')
		attr->ptr++;
	if (!*attr->ptr)
		return (0);
	new_token = get_single_char_token(attr);
	if (new_token)
		return (attr->ptr++, new_token);
	return (get_wrd_token(attr));
}
