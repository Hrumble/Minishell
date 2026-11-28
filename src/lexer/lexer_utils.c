/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/29 21:17:48 by ngero             #+#    #+#             */
/*   Updated: 2026/11/21 18:24:12 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	is_in(int c, char *buf)
{
	while (*buf)
		if (c == *buf++)
			return (1);
	return (0);
}

int	ft_isvar(int c)
{
	return (ft_isalnum(c) || c == '_' || c == '?');
}

int	ft_iswrd(int c)
{
	return (c && !is_in(c, "| !&<>"));
}

int	ft_isquotes(int c)
{
	return (c == '\'' || c == '"');
}

