/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 13:25:13 by samatsum          #+#    #+#             */
/*   Updated: 2025/04/01 13:44:27 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include "minishell.h"

int				exec(t_node *node, t_context *ctx);
static pid_t	exec_pipeline(t_node *node, t_context *ctx);
static void		exec_nonbuiltin(t_node *node, t_context *ctx)
				__attribute__((noreturn));
static void		validate_access(const char *path, const char *filename_token, \
	const char *tmp);

int	exec(t_node *node, t_context *ctx)
{
	pid_t	last_pid;
	int		status;
	char	**argv;

	open_redir_file(node, ctx);
	argv = token_list_to_argv(node->cmd_node->args_token);
	node->process_pid = -1;
	if (node->next == NULL && is_builtin(node) && !ft_strcmp(argv[0], "exit"))
		status = exec_builtin(node, ctx);
	else if (node->next == NULL && is_builtin(node) && !ft_strcmp(argv[0], "export"))
		status = exec_builtin(node, ctx);
	else if (node->next == NULL && is_builtin(node) && !ft_strcmp(argv[0], "cd"))
		status = exec_builtin(node, ctx);
	else if (node->next == NULL && is_builtin(node) && !ft_strcmp(argv[0], "unset"))
		status = exec_builtin(node, ctx);
	else
	{
		last_pid = exec_pipeline(node, ctx);
		wait_pipeline(last_pid, &status);
	}
	free_argv(argv);
	return (status);
}

/* ************************************************************************** */
static pid_t	exec_pipeline(t_node *node, t_context *ctx)
{
	if (node == NULL)
		return (-1);
	if (!node->cmd_node->args_token->word && node->cmd_node->args_token->next)
		node->cmd_node->args_token = node->cmd_node->args_token->next;
	prepare_pipe(node);
	node->process_pid = fork();
	if (node->process_pid < 0)
		fatal_error("fork");
	else if (node->process_pid == 0)
	{
		reset_signal();
		prepare_pipe_child(node);
		if (is_builtin(node))
			exit(exec_builtin(node, ctx));
		else
			exec_nonbuiltin(node, ctx);
	}
	prepare_pipe_parent(node);
	if (node->next)
		return (exec_pipeline(node->next, ctx));
	return (node->process_pid);
}

/* ************************************************************************** */
static void	exec_nonbuiltin(t_node *node, t_context *ctx)
{
	char		*path;
	char		**argv;

	do_redirect(node->cmd_node->redirects_node, ctx);
	argv = token_list_to_argv(node->cmd_node->args_token);
	path = argv[0];
	if (path != NULL)
		if (ft_strchr(path, '/') == NULL)
			path = search_path(path, ctx);
	validate_access(path, argv[0], node->cmd_node->args_token->word);
	execve(path, argv, get_environ(ctx->envmap));
	// printf("Fail_Execve\n");
	free_argv(argv);
	reset_redirect(node->cmd_node->redirects_node);
	if (path == NULL)
	{
		reset_redirect(node->cmd_node->redirects_node);
		exit(0);
	}
	fatal_error("execve");
}

/* ************************************************************************** */
static void	validate_access(const char *path, const char *filename_word, \
	const char *tmp)
{
	struct stat	st;

	if (path == NULL)
	{
		if (tmp != NULL)
			err_exit(filename_word, "01command not found", NOT_FOUND);
		else
			exit(0);
	}
	if (ft_strcmp(filename_word, "") == 0)
		err_exit(filename_word, "02command not found", NOT_FOUND);
	if (ft_strcmp(filename_word, "..") == 0)
		err_exit(filename_word, "03command not found", NOT_FOUND);
	if (ft_strcmp(filename_word, ".") == 0)
		err_exit(filename_word, "filename argument required", 2);
	if (access(path, F_OK) < 0)
		err_exit(filename_word, "No such file or directory", NOT_FOUND);
	if (stat(path, &st) < 0)
		fatal_error("fstat");
	if (S_ISDIR(st.st_mode))
		err_exit(filename_word, "Is a directory", PERMISSION_DENIED);
	if (access(path, X_OK) < 0)
		err_exit(path, "Permission denied", PERMISSION_DENIED);
}
