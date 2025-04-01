/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 13:51:36 by samatsum          #+#    #+#             */
/*   Updated: 2025/04/01 21:20:00 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "minishell.h"

t_node	*redirect_out(t_token **rest, t_token *tok);
t_node	*redirect_in(t_token **rest, t_token *tok);
t_node	*redirect_append(t_token **rest, t_token *tok);
t_node	*redirect_heredoc(t_token **rest, t_token *tok);

/* ************************************************************************** */
t_node	*redirect_out(t_token **rest, t_token *tok)
{
	t_node	*node;

	node = new_node(NODE_REDIR_OUT);
	node->filename_token = tokdup(tok->next);
	node->filename_token->original_word = tok->next->original_word;
	node->out_fd = STDOUT_FILENO;
	*rest = tok->next->next;
	return (node);
}

/* ************************************************************************** */
t_node	*redirect_in(t_token **rest, t_token *tok)
{
	t_node	*node;

	node = new_node(NODE_REDIR_IN);
	node->filename_token = tokdup(tok->next);
	node->filename_token->original_word = tok->next->original_word;
	node->out_fd = STDIN_FILENO;
	*rest = tok->next->next;
	return (node);
}

/* ************************************************************************** */
t_node	*redirect_append(t_token **rest, t_token *tok)
{
	t_node	*node;

	node = new_node(NODE_REDIR_APPEND);
	node->filename_token = tokdup(tok->next);
	node->filename_token->original_word = tok->next->original_word;
	node->out_fd = STDOUT_FILENO;
	*rest = tok->next->next;
	return (node);
}

/* ************************************************************************** */
t_node	*redirect_heredoc(t_token **rest, t_token *tok)
{
	t_node	*node;

	node = new_node(NODE_REDIR_HEREDOC);
	node->delimiter_token = tokdup(tok->next);
	if (ft_strchr(node->delimiter_token->word, SINGLE_QUOTE_CHAR) == NULL
		&& ft_strchr(node->delimiter_token->word, DOUBLE_QUOTE_CHAR) == NULL)
		node->is_delim_unquoted = true;
	node->out_fd = STDIN_FILENO;
	*rest = tok->next->next;
	return (node);
}
