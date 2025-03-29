/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_word_splitting.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 21:43:23 by samatsum          #+#    #+#             */
/*   Updated: 2025/03/29 17:30:06 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "xlib.h"
#include "minishell.h"

void		expand_word_splitting(t_node *node, t_context *ctx);
static void	expand_word_splitting_tok(t_token *tok, t_context *ctx);
static void	word_split(t_token *tok, t_context *ctx);
static void	process_char(char **new_word, char **p, t_token **tok, \
	t_context *ctx);
static void	insert_new_tok(char **new_word, t_token **rest, t_token *tok);

/* ************************************************************************** */
void	expand_word_splitting(t_node *node, t_context *ctx)
{
	if (node == NULL)
		return ;
	expand_word_splitting_tok(node->args_token, ctx);
	expand_word_splitting_tok(node->filename_token, ctx);
	expand_word_splitting(node->redirects_node, ctx);
	expand_word_splitting(node->cmd_node, ctx);
	expand_word_splitting(node->next, ctx);
}

/* ************************************************************************** */
static void	expand_word_splitting_tok(t_token *tok, t_context *ctx)
{
	if (tok == NULL || tok->kind != TOKEN_WORD || tok->word == NULL)
		return ;
	if (tok->export_flag == 1 && tok->next && tok->next->word)
		if (is_alpha_under(tok->next->word[0]))
			tok->next->export_flag = 2;
	if (!tok->non_double_quote_valuable)
		return (expand_word_splitting_tok(tok->next, ctx));
	word_split(tok, ctx);
	expand_word_splitting_tok(tok->next, ctx);
}

static void	word_split(t_token *tok, t_context *ctx)
{
	char	*new_word;
	char	*p;
	char	*word_copy;

	if (!tok->word)
		return ;
	word_copy = xstrdup(tok->word);
	if (tok->export_flag != 2)
		trim_ifs(&p, word_copy, ctx);
	else
		p = word_copy;
	new_word = xcalloc(1, sizeof(char));
	while (p && *p)
		process_char(&new_word, &p, &tok, ctx);
	free(tok->word);
	tok->word = new_word;
	free(word_copy);
}

static void	process_char(char **new_word, char **p, t_token **tok, \
	t_context *ctx)
{
	if (**p == SINGLE_QUOTE_CHAR)
		append_single_quote(new_word, p, *p);
	else if (**p == DOUBLE_QUOTE_CHAR)
		append_double_quote(new_word, p, *p, ctx);
	else if ((*tok)->export_flag != 2 && consume_ifs(p, *p, ctx) && *p && **p)
		insert_new_tok(new_word, tok, *tok);
	else if (*p && **p)
		append_char(new_word, *(*p)++);
}

static void	insert_new_tok(char **new_word, t_token **rest, t_token *tok)
{
	t_token	*new_tok;

	free(tok->word);
	tok->word = *new_word;
	*new_word = xcalloc(1, sizeof(char));
	new_tok = new_token(NULL, TOKEN_WORD);
	new_tok->next = tok->next;
	tok->next = new_tok;
	*rest = tok->next;
}
