/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 03:47:17 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/15 02:48:04 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	set_jc(t_lex *lexed)
{
	int	join_count;

	join_count = 0;
	while (lexed)
	{
		lexed->join_count = join_count;
		if (lexed->token == WHITE_SPACE)
		{
			lexed->join_count--;
			join_count++;
		}
		lexed = lexed->next;
	}
}

void	expand_quotes(t_lex *lex)
{
	char	*str;
	t_lex	*looping;
	t_lex	*original;

	original = lex;
	str = NULL;
	lex = lex->next;
	looping = lex;
	while (looping && looping->token != original->token)
		looping = looping->next;
	while (lex && lex != looping)
	{
		str = ft_strjoin(str, lex->content, GC);
		lex = lex->next;
	}
	if (!str)
		str = ft_strdup("", GC);
	original->content = str;
	original->to_replace = REPLACE_ALL;
	if (original->token == DOUBLE_QUOTE)
		original->to_replace = ONLY_ENV;
	else if (original->token == QUOTE)
		original->to_replace = NO_REPLACE;
	original->token = WORD;
	original->next = looping->next;
}

void	quotes_handler(t_lex *lexed)
{
	t_lex	*original;

	original = lexed;
	while (lexed)
	{
		if (lexed->token == QUOTE || lexed->token == DOUBLE_QUOTE)
			expand_quotes(lexed);
		lexed = lexed->next;
	}
	set_jc(original);
}
