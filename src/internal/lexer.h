/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/28 17:47:12 by ngero             #+#    #+#             */
/*   Updated: 2026/10/29 21:41:24 by ngero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H
# include <stddef.h>

typedef enum e_tokentype
{
	TK_WRD,
	TK_QUOTES,
	TK_BKQUOTES,
	TK_BKSLASH,
	TK_PIPE,
	TK_REDIROUT,
	TK_REDIRIN,
	TK_HEREDOC,
	TK_APPENDOUT,
}	t_tokentype;

// The `buf` stands for the entire buffer.
// The `ptr` is the ptr to the current lexer position within the buffer
// The `flag` is a list of flags for the current status
// flags (from right to left):
//
// 0: is_in_quotes
// 1: is_in_wrd
typedef struct s_lexerattr
{
	char	*buf;
	char	*ptr;
	int		flags : 2;
}	t_lexerattr;

typedef struct s_atom
{
	char	*ptr;
	size_t	size;
}	t_atom;

typedef struct s_token
{
	t_tokentype	type;
	union
	{
		t_atom	atom;
	};
}	t_token;

// Returns the next token in the string
t_token		*next_token(t_lexerattr *attr);
// Returns a new heap allocated token struct
t_token		*new_token(t_tokentype type, char *ptr, size_t size);
// Returns a new allocated lexer attribute
t_lexerattr	*new_lexerattr(char *buf);
int			ft_iswrd(int c);

#endif
