/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/26 17:58:51 by ngero             #+#    #+#             */
/*   Updated: 2026/10/26 18:24:06 by ngero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ft_printf.h"
#include "libft.h"
#include "terminal.h"
#include <stddef.h>
#include <unistd.h>

void	print_buffer(char *buf, size_t buf_size)
{
	size_t	i;

	ft_printf("Buffer(%d): ", buf_size);
	i = 0;
	while (i < buf_size)
	{
		if (ft_isprint(buf[i]))
			ft_putchar(buf[i]);
		else
			ft_printf("0x%x", buf[i]);
		i++;
	}
	ft_putstr("\n");
}

void	handle_commands(char *command, size_t command_size, t_lineinput *input)
{
	if (!ft_strncmp(command, "\033[C", command_size))
	{
		if (input->buf_ptr >= input->buffer + input->buf_size)
			return ;
		write(1, command, command_size);
		input->buf_ptr += 1;
	}
	if (!ft_strncmp(command, "\033[D", command_size))
	{
		if (input->buf_ptr <= input->buffer)
			return ;
		write(1, command, command_size);
		input->buf_ptr -= 1;
	}
}

void	insert_char_in_buf(char c, t_lineinput *input)
{
	size_t	l_until_eol;

	l_until_eol = (input->buffer + input->buf_size) - input->buf_ptr;
	if (!input)
		return ;
	if (l_until_eol)
		ft_memmove(input->buf_ptr+1, input->buf_ptr, l_until_eol);
	*input->buf_ptr = c;
	write(1, &c, 1);
	input->buf_ptr += 1;
	terminal_send_command("[K");
	write(1, input->buf_ptr, l_until_eol);
	input->buf_size += 1;
	if (l_until_eol)
		terminal_send_command_count("D", l_until_eol);
}

int	get_user_input(char *prompt, char *buf)
{
	char	i_buf[10];
	ssize_t	n_read;
	t_lineinput	input;

	ft_putstr_fd(prompt, 1);
	input.buffer = buf;
	input.buf_size = 0;
	input.buf_ptr = buf;
	n_read = read(STDIN_FILENO, i_buf, 10);
	while (n_read > 0 && *i_buf != '\n')
	{
		if (*i_buf == '\033')
			handle_commands(i_buf, n_read, &input);
		else
			insert_char_in_buf(*i_buf, &input);
		n_read = read(STDIN_FILENO, i_buf, 10);
	}
	write(1, "\n", 1);
	buf[input.buf_size] = 0;
	print_buffer(buf, input.buf_size);
	return (1);
}

int	minishell()
{
	char	buf[1024];

	terminal_set_icanon(0);
	while (1)
		get_user_input("metasex> ", buf);
	return (1);
}
