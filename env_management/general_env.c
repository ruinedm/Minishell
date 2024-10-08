/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 18:00:20 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/19 09:44:51 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env_management.h"

void	swap(t_env *a, t_env *b)
{
	char	*temp;

	temp = a->value;
	a->value = b->value;
	b->value = temp;
}

void	sort_env_list(t_env *head)
{
	int		swapped;
	t_env	*ptr1;
	t_env	*lptr;

	lptr = NULL;
	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		ptr1 = head;
		while (ptr1 && ptr1->next != lptr)
		{
			if (ft_strcmp(ptr1->value, ptr1->next->value) > 0)
			{
				swap(ptr1, ptr1->next);
				swapped = 1;
			}
			ptr1 = ptr1->next;
		}
		lptr = ptr1;
	}
}

int	no_envp_export(t_env **env, t_data *data)
{
	char	*pwd;

	if (data->pwd)
	{
		pwd = ft_strjoin("PWD=", data->pwd, MANUAL);
		null_protector(pwd);
		exp_w_null(env, pwd);
		free(pwd);
	}
	exp_w_null(env, "SHLVL=1");
	exp_w_null(env, SECURE_PATH);
	return (0);
}
