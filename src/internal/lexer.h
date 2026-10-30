/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/28 17:47:12 by ngero             #+#    #+#             */
/*   Updated: 2026/10/30 17:09:39 by ngero            ###   ########.fr       */
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
	TK_REDIR,
}	t_tokentype;

typedef enum e_redirtype
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPENDOUT,
	REDIR_HEREDOC,
}	t_redirtype;

// The `buf` stands for the entire buffer.
// The `ptr` is the ptr to the current lexer position within the buffer
// The `flag` is a list of flags for the current status
// flags (from right to left):
//
// 0: is_in_quotes
typedef struct s_lexerattr
{
	char	*buf;
	char	*ptr;
	int		flags : 1;
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
		t_atom		atom;
		t_redirtype	redir_type;
	};
}	t_token;

// Returns the next token in the string
t_token		*next_token(t_lexerattr *attr);
// Returns a new heap allocated token struct
t_token		*new_token(t_tokentype type, char *ptr, size_t size);
// Returns a new heap allocated redir token
t_token		*new_token_redir(t_redirtype type);
// Returns a new allocated lexer attribute
t_lexerattr	*new_lexerattr(char *buf);
int			ft_iswrd(int c);

#endif
