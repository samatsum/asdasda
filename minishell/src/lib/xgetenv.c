/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xgetenv.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 15:34:48 by samatsum          #+#    #+#             */
/*   Updated: 2025/03/31 19:29:00 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ************************************************************************** */
char	*xgetenv(const char *name, t_map *envmap)
{
	t_item	*item;

	item = map_get(envmap, name);
	if (item == NULL)
		return (NULL);
	return (item->value);
}
