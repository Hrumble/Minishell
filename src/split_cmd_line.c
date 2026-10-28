/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmd_line.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/27 19:40:36 by ngero             #+#    #+#             */
/*   Updated: 2026/10/27 20:42:22 by ngero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>

// Inner loop of the arg counter. 
// Walks while *c is ' ' && *c != '\0' and we're not in quotes setting is_in = 0
// walks while *c != '\0' AND either (this is not a space) OR (we're in quotes)
// On the second walk, check on each loop wether or not the next char is quotes
static void	arg_count_inner(const char **c, char **word_p, int *is_quotes)
{
	while (**c == ' ' && **c && !*is_quotes)
	{
		*word_p = 0;
		(*c)++;
		*is_quotes = **c == '\"';
	}
	while ((**c != ' '|| *is_quotes) && **c)
	{
		if (!*word_p)
			*word_p = (char *)*c;
		(*c)++;
		if (*is_quotes && **c == '\"')
		{
			*is_quotes = 0;
			(*c)++;
			break;
		}
	}
}

static int	arg_count_cmdline(const char *c)
{
	char	*word_p;
	int		count;
	int		is_quotes;

	is_quotes = 0;
	count = 0;
	word_p = 0;
	while (*c)
	{
		arg_count_inner(&c, &word_p, &is_quotes);
		if (word_p && !is_quotes)
			count++;
	}
	return (count);
}

void	*free_split(char **av)
{
	char	**sk;

	sk = av;
	if (!av)
		return (0);
	while (*sk)
		free(*sk++);
	free(av);
	return (0);
}

char	**split_cmdline(const char *c)
{
	char	**av;
	char	*word_p;
	int		is_quotes;
	int		current_index;

	is_quotes = 0;
	word_p = 0;
	av = malloc((arg_count_cmdline(c) + 1) * sizeof(char **));
	if (!av)
		return (0);
	current_index = 0;
	while (*c)
	{
		arg_count_inner(&c, &word_p, &is_quotes);
		if (word_p)
		{
			av[current_index] = ft_substr(word_p, 0, c - word_p);
			if (!av[current_index])
				return (free_split(av));
			current_index++;
		}
	}
	av[current_index] = 0;
	return (av);
}

int	count_split(const void **sp)
{
	int	size;

	if (!sp)
		return (0);
	size = 0;
	while (*sp)
	{
		sp++;
		size++;
	}
	return (size);
}
