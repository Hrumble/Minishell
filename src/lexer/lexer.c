/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/28 17:47:04 by ngero             #+#    #+#             */
/*   Updated: 2026/10/30 12:29:52 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include "lexer.h"

static t_token	*get_single_char_token(t_lexerattr *attr)
{
	if (*attr->ptr == '|')
		return (new_token(TK_PIPE, 0, 0));
	if (*attr->ptr == '<')
	{
		attr->ptr++;
		if (*attr->ptr == '<')
			return (new_token_redir(REDIR_HEREDOC));
		return (new_token_redir(REDIR_IN));
	}
	if (*attr->ptr == '>')
	{
		attr->ptr++;
		if (*attr->ptr == '>')
			return (new_token_redir(REDIR_APPENDOUT));
		return (new_token_redir(REDIR_OUT));
	}
	if (*attr->ptr == '`')
		return (new_token(TK_BKQUOTES, 0, 0));
	if (*attr->ptr == '\\')
		return (new_token(TK_BKSLASH, 0, 0));
	return (0);
}


static int	handle_quotes(t_lexerattr *attr)
{
	if (*attr->ptr == '"' || *attr->ptr == '\'')
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
	
	wrd_start = 0;
	handle_quotes(attr);
	while ((ft_iswrd(*attr->ptr) || attr->flags & 1) && *attr->ptr)
	{
		if (!wrd_start)
			wrd_start = attr->ptr;
		attr->ptr++;
		if (handle_quotes(attr))
			break ;
	}
	if (!wrd_start)
		return (0);
	return new_token(TK_WRD, wrd_start, attr->ptr - wrd_start);
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
