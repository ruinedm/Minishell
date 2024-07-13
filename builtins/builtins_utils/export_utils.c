/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 00:09:24 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/13 01:13:04 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../builtins.h"

bool	no_equal(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
			return (false);
		i++;
	}
	return (true);
}

bool	underscore_before_equal(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '=')
	{
		if (str[i] != '_')
			break ;
		i++;
	}
	if (str[i] && str[i] == '=')
		return (true);
	return (false);
}

bool	initial_check(char *str, int i, int count)
{
	while (str[i] && str[i] != '+' && str[i] != '=')
	{
		if (!ft_isalpha(str[i]) && !is_c_num(str[i]))
			return (false);
		i++;
	}
	if (str[i] == '+')
	{
		if (!str[i + 1])
			return (false);
		if (str[i + 1] != '=')
			return (false);
		i++;
	}
	while (str[i])
	{
		if (str[i] == '=')
			count++;
		i++;
	}
	if (count > 1)
		return (false);
	return (true);
}

int	is_append(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	if (!str[i])
		return (false);
	if (str[i - 1] == '+')
		return (true);
	return (false);
}

int	get_export_type(char *str, int i)
{
	if ((str[0] == '?' && str[1] == '='))
		return (1);
	if (str[0] == '=')
		return (0);
	if (underscore_before_equal(str))
		return (1);
	if (is_c_num(str[0]) || !initial_check(str, 0, 0))
		return (0);
	if (no_equal(str))
		return (3);
	if (is_append(str))
		return (4);
	while (str[i] && str[i] != '=')
	{
		if (ft_isalpha(str[i]))
			break ;
		i++;
	}
	if (str[i] && str[i] == '=')
		return (0);
	if (!str[i] || (str[i] == '=' && !str[i + 1]))
		return (2);
	return (1);
}