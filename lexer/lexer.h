/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 01:51:04 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/13 03:45:38 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "../minishell.h"

t_lex	*check_redirections(t_lex *tokens);
t_lex	*check_logical_operators(t_lex *tokens);
void	handle_double_special(char *input, int *i, t_lex **head,
			int *join_count);
void	handle_star(char *input, int *i, t_lex **head, int *join_count);
void	handle_env(char *input, int *i, t_lex **head, int *join_count);
void	handle_general_special(char *input, int *i,
			t_lex **head, int *join_count);
void	handle_word(char *input, int *i, t_lex **head, int *join_count);
#endif