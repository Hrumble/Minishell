/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bit_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <ngero@student.codam.nl>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/10 21:22:53 by user              #+#    #+#             */
/*   Updated: 2026/11/10 22:02:31 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	get_bit(int field, int pos)
{
	return (field >> pos & 1);
}

void	set_bit(int *field, int pos)
{
	*field |= 1 << pos;
}

void	clear_bit(int *field, int pos)
{
	*field &= ~(1 << pos);
}
