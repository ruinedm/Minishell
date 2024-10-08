/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_tokenizer.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 17:56:42 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/19 09:45:28 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env_management.h"

void	heredoc_handle_env(char *input, int *i, t_lex **head)
{
	t_lex	*current_node;
	int		hold;
	int		c;
	char	*content;

	c = input[*i];
	hold = *i;
	(*i)++;
	while (input[*i] && input[*i] != '$' && !is_ws(input[*i])
		&& input[*i] != '.' && !is_special(input[*i]))
		(*i)++;
	content = ft_substr(input, hold, *i - hold, GC);
	current_node = ft_lstnew_lex(content, c, *i - hold);
	ft_lstadd_back_lex(head, current_node);
}

void	heredoc_handle_word(char *input, int *i, t_lex **head)
{
	t_lex	*current_node;
	char	*content;
	int		hold;
	int		type;

	hold = *i;
	type = COMMAND;
	while (input[*i] && input[*i] != '$')
		(*i)++;
	content = ft_substr(input, hold, *i - hold, GC);
	current_node = ft_lstnew_lex(content, type, *i - hold);
	ft_lstadd_back_lex(head, current_node);
}

t_lex	*heredoc_tokenizer(char *input)
{
	int		i;
	t_lex	*head;
	int		join_count;

	head = NULL;
	i = 0;
	join_count = 0;
	while (input[i])
	{
		if (input[i] == '$')
			heredoc_handle_env(input, &i, &head);
		else
			heredoc_handle_word(input, &i, &head);
	}
	set_jc(head);
	return (head);
}
