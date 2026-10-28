/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/28 17:47:12 by ngero             #+#    #+#             */
/*   Updated: 2026/10/28 18:05:50 by ngero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

typedef enum e_tokentype {
	WRD,
	PIPE,
	REDIROUT,
	REDIRIN,
} t_tokentype;

typedef struct s_lexerattr {
	char	*str;
	char	*pos;
} t_lexerattr;

typedef struct s_token {
	t_tokentype	type;
	char		*value;
} t_token;

// Returns the next token in the string
t_token	*next_token(t_lexerattr *attr);
// Returns a new heap allocated token struct
t_token	*new_token(t_tokentype type, char *v);

#endif
