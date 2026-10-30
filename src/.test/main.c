/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/29 17:19:03 by ngero             #+#    #+#             */
/*   Updated: 2026/10/30 19:01:09 by ngero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "debug.h"
#include "ft_printf.h"
#include "parser.h"
#include <stdlib.h>

int	main()
{
	char		str[] = "echo \"fuck your mom\" > in >";
	t_ast		*ast;

	ast = parse_ast(str);
	print_ast(ast);
}
