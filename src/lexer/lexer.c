/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/28 17:47:04 by ngero             #+#    #+#             */
/*   Updated: 2026/10/29 21:41:33 by ngero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include "lexer.h"

static t_tokentype	get_single_char_token(t_lexerattr *attr)
{
	if (*attr->ptr == '|')
		return (TK_PIPE);
	if (*attr->ptr == '<')
	{
		attr->ptr++;
		if (*attr->ptr == '<')
			return (TK_HEREDOC);
		return (TK_REDIRIN);
	}
	if (*attr->ptr == '>')
	{
		attr->ptr++;
		if (*attr->ptr == '>')
			return (TK_APPENDOUT);
		return (TK_REDIROUT);
	}
	if (*attr->ptr == '`')
		return (TK_BKQUOTES);
	if (*attr->ptr == '\\')
		return (TK_BKSLASH);
	return (0);
}


static int	handle_quotes(t_lexerattr *attr)
{
	if (*attr->ptr == '"')
	{
		if (attr->flags & 1)
		{
			attr->flags ^= 1;
			attr->ptr++;
			return (1);
		}
		attr->flags |= 1;
	}
	return (0);
}

static t_token	*get_wrd_token(t_lexerattr *attr)
{
	char	*wrd_start;
	int		is_quotes;
	
	is_quotes = 0;
	wrd_start = 0;
	if (*attr->ptr == '"')
	{
		attr->flags |= 1;
		attr->ptr++;
	}
	while ((ft_iswrd(*attr->ptr) || attr->flags & 1) && *attr->ptr)
	{
		if (!wrd_start)
			wrd_start = attr->ptr;
		attr->ptr++;
		is_quotes = handle_quotes(attr);
		if (is_quotes)
			break ;
	}
	if (!wrd_start)
		return (0);
	return new_token(TK_WRD, wrd_start, (attr->ptr - is_quotes) - wrd_start);
}

t_token	*next_token(t_lexerattr *attr)
{
	t_tokentype	ttype;

	while (*attr->ptr == ' ')
		attr->ptr++;
	if (!*attr->ptr)
		return (0);
	ttype = get_single_char_token(attr);
	if (ttype)
		return (attr->ptr++, new_token(ttype, 0, 0));
	return (get_wrd_token(attr));
}
