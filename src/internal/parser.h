/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/29 19:53:19 by ngero             #+#    #+#             */
/*   Updated: 2026/10/29 22:47:19 by ngero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H
# define ARGV_SIZE 100
# include "lexer.h"
#include <stddef.h>

typedef struct s_parserattr
{
	t_token	*prev;
	t_token	*current;
	t_token	*next;
}	t_parserattr;

typedef enum e_asttype
{
	AST_PRGRM,
	AST_BINARYOP,
}	t_asttype;

typedef enum e_binaryop
{
	BOP_PIPE,
}	t_binaryop;

typedef enum e_redirtype
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPENDIN,
	REDIR_HEREDOC,
}	t_redirtype;

typedef struct s_astredir
{
	t_redirtype	type;
	char		*target;
} t_astredir;

typedef struct s_ast
{
	t_asttype	type;
	union
	{
		struct
		{
			char		*argv[ARGV_SIZE];
			int			ac;
			t_astredir	**redir;
		}	s_astprgrm;
		struct
		{
			t_binaryop		binaryop;
			struct s_ast	*left;
			struct s_ast	*right;
		}	s_binaryop;
	};
}	t_ast;

// Consumes a token
t_token			*consume_token(t_parserattr *pattr, t_lexerattr *lattr);
// Is the current token a `type`
int				peek(t_parserattr *pattr, t_tokentype type);
// Parses an input string
t_ast			*parse_ast(char	*buf);

// Constructors
t_parserattr	*new_parserattr();
t_ast			*new_astprgrm();

#endif
