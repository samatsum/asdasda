/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ_map_put.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 17:32:54 by samatsum          #+#    #+#             */
/*   Updated: 2025/03/31 20:50:35 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "xlib.h"
#include "minishell.h"

/* ************************************************************************** */
int			map_put(t_map *envmap, const char *string, bool allow_empty, int attr);
static void	set_name_value(t_map *envmap, const char *string, char **name, char **value);

/* ************************************************************************** */
int	map_put(t_map *envmap, const char *string, bool allow_empty, int attr)
{
	int		result;
	char	*name;
	char	*value;

	set_name_value(envmap, string, &name, &value);
	if (value == NULL && !allow_empty)
	{
		free(name);
		name = NULL;
		return (-1);
	}
	result = map_set_attr(envmap, name, value, attr);
	free(name);
	name = NULL;
	free(value);
	value = NULL;
	return (result);
}

/* ************************************************************************** */
static void	set_name_value(t_map *envmap, const char *string, char **name, char **value)
{
	char	*name_end;
	char	*name_end_append;
	char	*original_value;

	name_end = ft_strchr(string, '=');
	name_end_append = ft_strnstr(string, "+=", ft_strlen(string));
	if (name_end == NULL)
	{
		*name = xstrdup(string);
		*value = NULL;
	}
	else if (name_end && name_end_append == NULL)
	{
		*name = xstrndup(string, name_end - string);
		*value = xstrdup(name_end + 1);
	}
	else
	{
		*name = xstrndup(string, name_end_append - string);
		original_value = xgetenv(*name, envmap);
		*value = xstrdup(name_end_append + 2);
		if (original_value)
			*value = ft_strjoin(original_value, *value);
	}
}
