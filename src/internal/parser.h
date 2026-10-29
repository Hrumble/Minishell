/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/29 19:53:19 by ngero             #+#    #+#             */
/*   Updated: 2026/10/29 21:41:15 by ngero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H
# include "lexer.h"

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
	BOP_REDIRIN,
	BOP_REDIROUT,
}	t_binaryop;

typedef struct s_ast
{
	t_asttype	type;
	union
	{
		struct
		{
			t_atom			**argv;
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
t_parserattr	*new_parserattr();

#endif
