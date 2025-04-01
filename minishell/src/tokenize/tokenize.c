/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 14:11:33 by samatsum          #+#    #+#             */
/*   Updated: 2025/04/01 16:17:36 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "xlib.h"
#include "minishell.h"

t_token			*tokenize(char *head, t_context *ctx);
static bool		skip_space_and_check_empty(char **next_non_blank, char *head);
bool			is_metacharacter(char c);
static t_token	*operator(char **next_non_oprators, char *head);
static int		set_operator_token(char *op);

/* ************************************************************************** */
t_token	*tokenize(char *line, t_context *ctx)
{
	t_token	head;
	t_token	*tok;

	ctx->syntax_error = false;
	head.next = NULL;
	tok = &head;
	tok->kind = TOKEN_WORD;
	while (*line)
	{
		if (skip_space_and_check_empty(&line, line))
			continue ;
		if (*line && is_metacharacter(*line))
			tok->next = operator(&line, line);
		else if (*line && !is_metacharacter(*line))
			tok->next = word(&line, line, ctx);
		tok = tok->next;
	}
	if (head.next && head.next->kind == TOKEN_PIPE)
		tokenize_error("Unexpected Token", &line, head.next, ctx);
	else if (tok->kind != TOKEN_WORD)
		tokenize_error("Unexpected Token", &line, tok, ctx);
	tok->next = new_token(NULL, TOKEN_NULL);
	return (head.next);
}

/* ************************************************************************** */
static bool	skip_space_and_check_empty(char **next_non_blank, char *head)
{
	while (*head && (*head == ' ' || *head == '\t' || *head == '\n'))
		head++;
	*next_non_blank = head;
	return (*head == '\0');
}

/* ************************************************************************** */
bool	is_metacharacter(char c)
{
	return (ft_strchr(" \t\n|<>", c) != NULL);
}

/* ************************************************************************** */
static t_token	*operator(char **next_non_oprators, char *head)
{
	static char *const	operators[] = {">>", "<<", "<", ">", "|", "\n"};
	size_t				i;				
	char				*op;
	int					op_token_kind;

	i = 0;
	while (i < sizeof(operators) / sizeof(*operators))
	{
		if (startswith(head, operators[i]))
		{
			op = xstrdup(operators[i]);
			*next_non_oprators = head + ft_strlen(op);
			op_token_kind = set_operator_token(op);
			return (new_token(op, op_token_kind));
		}
		i++;
	}
	assert_error("Unexpected operator");
}

/* ************************************************************************** */
static int	set_operator_token(char *op)
{
	if (ft_strncmp(op, "|", 2) == 0)
		return (TOKEN_PIPE);
	else if (ft_strncmp(op, ">", 2) == 0)
		return (TOKEN_REDIR_OUT);
	else if (ft_strncmp(op, "<", 2) == 0)
		return (TOKEN_REDIR_IN);
	else if (ft_strncmp(op, ">>", 3) == 0)
		return (TOKEN_REDIR_APPEND);
	else if (ft_strncmp(op, "<<", 3) == 0)
		return (TOKEN_REDIR_HEREDOC);
	else if (ft_strncmp(op, "\n", 2) == 0)
		return (TOKEN_NEWLINE);
	return (TOKEN_UNKNOWN);
}
