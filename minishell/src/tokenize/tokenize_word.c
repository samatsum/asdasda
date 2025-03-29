/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_word.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 19:12:31 by samatsum          #+#    #+#             */
/*   Updated: 2025/03/29 17:29:00 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "xlib.h"
#include "minishell.h"

t_token		*word(char **rest, char *line, t_context *ctx);
static void	validate_and_advance_quote(char **rest, char *line, t_context *ctx, \
	char quote_char);

/* ************************************************************************** */
t_token	*word(char **rest, char *head, t_context *ctx)
{
	const char	*start = head;
	char		*word;

	while (*head && !is_metacharacter(*head))
	{
		if (*head == SINGLE_QUOTE_CHAR)
			validate_and_advance_quote(&head, head, ctx, SINGLE_QUOTE_CHAR);
		else if (*head == DOUBLE_QUOTE_CHAR)
			validate_and_advance_quote(&head, head, ctx, DOUBLE_QUOTE_CHAR);
		else
			head++;
	}
	word = xstrndup(start, head - start);
	*rest = head;
	return (new_token(word, TOKEN_WORD));
}

/* ************************************************************************** */
static void	validate_and_advance_quote(char **rest, char *head, t_context *ctx, \
	char quote_char)
{
	char	*error_message;

	head++;
	while (*head && *head != quote_char)
	head++;
	if (*head == '\0')
	{
		if (quote_char == SINGLE_QUOTE_CHAR)
			error_message = "Unclosed single quote";
		else if (quote_char == DOUBLE_QUOTE_CHAR)
			error_message = "Unclosed double quote";
		else
			error_message = "Unclosed quote";
		unclose_error(error_message, &head, head, ctx);
	}
	else
		head++;
	*rest = head;
}
