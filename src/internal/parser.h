/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/29 19:53:19 by ngero             #+#    #+#             */
/*   Updated: 2026/11/05 14:44:29 by ngero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H
# define ARGV_SIZE 100
# define REDIR_SIZE 10
# include "lexer.h"
# include <stddef.h>

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

typedef struct s_prgrmredir
{
	t_redirtype	type;
	char		*target;
}	t_prgrmredir;

typedef struct s_ast
{
	t_asttype	type;
	union
	{
		struct
		{
			char			*argv[ARGV_SIZE];
			int				ac;
			t_prgrmredir	*redir[REDIR_SIZE];
			int				redir_size;
		}	s_prgrm;
		struct
		{
			t_binaryop		binaryop;
			struct s_ast	*left;
			struct s_ast	*right;
		}	s_bop;
	};
}	t_ast;

// Consumes a token
t_token			*consume_token(t_parserattr *pattr, t_lexerattr *lattr);
// Is the current token a `type`
int				peek(t_parserattr *pattr, t_tokentype type);
// Parses an input string
t_ast			*parse_ast(char	*buf);
// Parses a redirection
int				parse_redir(t_parserattr *pattr, t_lexerattr *lattr,
					t_ast *prgrm);
// Calls function `f` on every ast node in the list
// ast_map expects a function that returns an int, 0 on failure and
// 1 on success
int				ast_map(t_ast	**ast, int (*f)(t_ast **ast));

// Constructors
t_parserattr	*new_parserattr(void);
t_ast			*new_astprgrm(void);
t_ast			*new_astbinaryop(t_ast *left, t_ast *right);
t_prgrmredir	*new_redir(t_redirtype type, char *buf, size_t len);

// Freeing
void			free_redir(t_prgrmredir *redir);
void			free_prgrm(t_ast	*prgrm);

// Utils
// Returns wether or not the program is empty
int				is_empty_prgrm(t_ast *prgrm);
// Copies the array of redir from a program into another
int				prgrm_redircpy(t_prgrmredir **dest, t_prgrmredir **src,
					int redir_size);
#endif
