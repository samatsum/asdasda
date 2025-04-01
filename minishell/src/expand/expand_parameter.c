/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_parameter.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 16:35:11 by samatsum          #+#    #+#             */
/*   Updated: 2025/04/01 13:19:24 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "xlib.h"
#include "minishell.h"

void		expand_parameter(t_node *node, t_context *ctx);
static void	expand_parameter_tok(t_token *tok, t_context *ctx);

/* ************************************************************************** */
void	expand_parameter(t_node *node, t_context *ctx)
{
	if (node == NULL)
		return ;
	expand_parameter_tok(node->args_token, ctx);
	expand_parameter_tok(node->filename_token, ctx);
	expand_parameter(node->redirects_node, ctx);
	expand_parameter(node->cmd_node, ctx);
	expand_parameter(node->next, ctx);
}

/* ************************************************************************** */
static void	expand_parameter_tok(t_token *tok, t_context *ctx)
{
	char	*new_word;
	char	*p;

	if (tok == NULL || tok->kind != TOKEN_WORD || tok->word == NULL)
		return ;
	p = tok->word;
	new_word = xcalloc(1, sizeof(char));
	while (*p)
	{
		if (*p == SINGLE_QUOTE_CHAR)
			append_single_quote(&new_word, &p, p);
		else if (*p == DOUBLE_QUOTE_CHAR)
			append_double_quote(&new_word, &p, p, ctx);
		else if (is_variable(p))
			(expand_variable_str(&new_word, &p, p, ctx), \
			tok->non_double_quote_valuable = true);
		else if (is_special_parameter(p))
			expand_special_parameter_str(&new_word, &p, p, ctx);
		else
			append_char(&new_word, *p++);
	}
	free(tok->word);
	tok->word = NULL;
	tok->word = new_word;
	expand_parameter_tok(tok->next, ctx);
}
