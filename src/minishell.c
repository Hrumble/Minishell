/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/26 17:58:51 by ngero             #+#    #+#             */
/*   Updated: 2026/10/30 11:54:08 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "debug.h"
#include "internal.h"
#include "minishell.h"
#include "parser.h"
#include "terminal.h"
#include <stddef.h>
#include <unistd.h>

int	minishell()
{
	char		buf[1024];

	terminal_set_icanon(0);
	while (1)
	{
		get_user_input("metasex> ", buf);
		print_ast(parse_ast(buf));
	}

	return (1);
}
