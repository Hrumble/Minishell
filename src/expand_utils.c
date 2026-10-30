/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 16:47:57 by ngero             #+#    #+#             */
/*   Updated: 2026/10/30 17:11:07 by ngero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*find_matching_char(char *quote_pos)
{
	char	*sk;

	sk = quote_pos + 1;
	while (*sk && *sk != *quote_pos)
		sk++;
	if (!*sk)
		return (0);
	return (sk);
}
