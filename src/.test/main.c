/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/29 17:19:03 by ngero             #+#    #+#             */
/*   Updated: 2026/10/30 11:57:07 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "debug.h"
#include "parser.h"

int	main()
{
	char		str[] = "echo \"fuck your mom\" > in > out << back";
	t_ast		*ast;
	
	ast = parse_ast(str);
	print_ast(ast);
}
