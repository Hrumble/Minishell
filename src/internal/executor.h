/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 16:02:26 by ngero             #+#    #+#             */
/*   Updated: 2026/11/15 17:47:49 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H
#include <sys/types.h>
# define PATH_SIZE 100
# define MAX_PROCESSES 100
# define HEREDOC_FILE_PATH "/tmp/mshell_heredoc"

#include "parser.h"

typedef struct s_defaultredir
{
	int	default_io[2];
}	t_defaultredir;

// Executes an ast, returns the pid of the fork on success
// 0 on failure
int		fork_and_exec(t_ast *ast, char **env, int fd[2], int prev);

// Finds a binary from relative path or PATH and puts it in **bin
// returns 0 if unable to find a binary, -1 if provided buf is dir
int		find_binary(char *buf, char **bin);

// Redirections

// sets up the read and write entries specified by a redir.
int		set_redirection_stream(t_prgrmredir *redir);
// Handles the heredocs redir. Returns the fd of the written contents
int		write_heredoc_file(t_prgrmredir *heredoc_redir);
// sets up the redirections for a program.
// if builtin is set to 1, kbfd and termfd will also be set within the struct
int		setup_prgrm_redirections(t_ast *ast);
// Restores the default 0 and 1 streams from the credirs struct
int		restore_default_streams(t_defaultredir *credirs);
// Saves the current 0 and 1 streams into the current redirs struct
int		save_default_streams(t_defaultredir *credirs);
// Pipes the specified file descriptor to the file specified on path
// returns the fd of the newly openened file.
// The mode is used to specify the mode in which to open the file.
// O_CREAT should be default
int		pipe_fd_to_file(int fd, char *path, int mode);
// Returns a function pointer to the builtin function which the specified name.
// 0 if none
int		(*get_builtin(char *buf))(char **, int, char **);
// Executes an entire pipeline. To be called on BOPs
int		execute_pipeline(t_ast	*pipeline, char **env);
int		set_io_streams(int fd[2]);
// Reads the heredoc and sets the io array
int		set_program_heredoc_io(t_ast *program, int io[2]);
// awaits every process in the list one by one
void	await_processes(pid_t *processes, int size);
// Runs a program
int		run(t_ast *ast, char **env, int io[2]);
// Runs a program when there is only a program in the pipeline
int		run_standalone_program(t_ast *prgrm, char **env);

// Utils

// Counts the total number of programs inside a pipeline
int		_count_pipeline_programs(t_ast *pipeline);
// Flattens a pipeline.
// Returns a null terminated array of each program to be ran in order.
// When freeing, do not free the ast nodes, they are references from the actual
// AST
t_ast	**flatten_pipeline(t_ast *pipeline);
// Closes io[0] and io[1] as long as they are not stdin and stdout
void	close_io(int io[2]);

// freeing stuff
void	free_split(char **split);

#endif
