/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/29 17:19:03 by ngero             #+#    #+#             */
/*   Updated: 2026/10/29 21:21:18 by ngero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tester.h"
#include "lexer.h"
#include <stdlib.h>

int	main()
{
	char			str[] = "Hello < world.txt | grep -Ev \"  *hey*  \" >> ass.md";
	t_lexerattr 	*attr = new_lexerattr(str);
	t_token			*tk;

	tk = next_token(attr);
	while (tk)
	{
		print_token(tk);
		free(tk);
		tk = next_token(attr);
	}
}
