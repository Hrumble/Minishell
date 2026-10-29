/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/29 17:19:03 by ngero             #+#    #+#             */
/*   Updated: 2026/10/29 23:00:57 by ngero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "tester.h"

int	main()
{
	char	str[] = "echo \"fuck your mom\" | cat -e";
	t_ast	*ast;
	
	ast = parse_ast(str);
	print_ast(ast);
}
