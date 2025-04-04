/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_open_redir_file.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 12:11:42 by samatsum          #+#    #+#             */
/*   Updated: 2025/04/01 21:19:12 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include "minishell.h"

int			open_redir_file(t_node *node, t_context *ctx);
int			openfd(t_node *node, t_context *ctx);
static int	handle_redir_error(t_node *node);

/* ************************************************************************** */
int	open_redir_file(t_node *node, t_context *ctx)
{
	if (node == NULL)
		return (0);
	if (node->kind == NODE_PIPELINE)
	{
		if (open_redir_file(node->cmd_node, ctx) < 0)
			return (-1);
		if (open_redir_file(node->next, ctx) < 0)
			return (-1);
		return (0);
	}
	else if (node->kind == NODE_SIMPLE_CMD)
		return (open_redir_file(node->redirects_node, ctx));
	node->from_fd = openfd(node, ctx);
	if (node->from_fd < 0)
		return (handle_redir_error(node));
	node->from_fd = stashfd(node->from_fd);
	return (open_redir_file(node->next, ctx));
}

/* ************************************************************************** */
int	openfd(t_node *node, t_context *ctx)
{
	if (node->kind != NODE_REDIR_HEREDOC)
		if (!node->filename_token || !node->filename_token->word)
			return (-1);
	if (node->kind == NODE_REDIR_OUT)
		return (open(node->filename_token->word, \
			O_CREAT | O_WRONLY | O_TRUNC, 0644));
	else if (node->kind == NODE_REDIR_IN)
		return (open(node->filename_token->word, O_RDONLY));
	else if (node->kind == NODE_REDIR_APPEND)
		return (open(node->filename_token->word,
				O_CREAT | O_WRONLY | O_APPEND, 0644));
	else if (node->kind == NODE_REDIR_HEREDOC)
		return (read_heredoc(node->delimiter_token->word, \
			node->is_delim_unquoted, ctx));
	else
		assert_error("open_redir_file");
}

static int	handle_redir_error(t_node *node)
{
	if (node->kind == NODE_REDIR_OUT || node->kind == NODE_REDIR_APPEND \
		|| node->kind == NODE_REDIR_IN)
	{
		if (node->filename_token && node->filename_token->word)
			xperror2(node->filename_token->word, NULL);
		else
			xperror2(node->filename_token->original_word, "ambiguous redirect");
	}
	return (-1);
}
