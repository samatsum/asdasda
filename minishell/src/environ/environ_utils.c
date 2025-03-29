/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 20:44:36 by samatsum          #+#    #+#             */
/*   Updated: 2025/03/29 17:30:28 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool		is_identifier(const char *s);
bool		item_exported(t_item *item);
t_item		*map_get(t_map *map, const char *name);
int			map_unset(t_map *map, const char *name);
static void	free_cur(t_item *cur);

/* ************************************************************************** */
bool	is_identifier(const char *s)
{
	if (!is_alpha_under(*s))
		return (false);
	s++;
	while (*s)
	{
		if (!is_alpha_num_under(*s))
			return (false);
		s++;
	}
	return (true);
}

/* ************************************************************************** */
bool	item_exported(t_item *item)
{
	return (item->attributes & ATTR_EXPORT);
}

/* ************************************************************************** */
t_item	*map_get(t_map *map, const char *name)
{
	t_item	*cur;

	if (name == NULL)
		return (NULL);
	cur = map->item_head.next;
	while (cur)
	{
		if (ft_strcmp(cur->name, name) == 0)
			return (cur);
		cur = cur->next;
	}
	return (NULL);
}

/* ************************************************************************** */
int	map_unset(t_map *map, const char *name)
{
	t_item	*cur;
	t_item	*prev;

	if (name == NULL)
		return (-1);
	if (!is_identifier(name))
		return (0);
	prev = &map->item_head;
	cur = map->item_head.next;
	while (cur)
	{
		if (ft_strcmp(cur->name, name) == 0)
		{
			prev->next = cur->next;
			free_cur(cur);
			return (0);
		}
		prev = prev->next;
		cur = cur->next;
	}
	return (0);
}

static void	free_cur(t_item *cur)
{
	free(cur->name);
	cur->name = NULL;
	free(cur->value);
	cur->value = NULL;
	free(cur);
	cur = NULL;
}
