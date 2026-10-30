/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enum_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <ngero@student.codam.nl>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 11:24:46 by user              #+#    #+#             */
/*   Updated: 2026/10/30 12:29:18 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "lexer.h"

void	tostr__t_tokentype(t_tokentype type)
{
	if (type == TK_WRD) ft_printf("TK_WRD");
	if (type == TK_PIPE) ft_printf("TK_PIPE");
	if (type == TK_QUOTES) ft_printf("TK_QUOTES");
	if (type == TK_BKQUOTES) ft_printf("TK_BKQUOTES");
	if (type == TK_BKSLASH) ft_printf("TK_BKSLASH");
	if (type == TK_REDIR) ft_printf("TK_REDIR");
}

void	tostr__t_redirtype(t_redirtype type)
{
	if (type == REDIR_IN) ft_printf("REDIRIN");
	if (type == REDIR_OUT) ft_printf("REDIROUT");
	if (type == REDIR_APPENDOUT) ft_printf("APPENDOUT");
	if (type == REDIR_HEREDOC) ft_printf("HEREDOC");
}
