/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   traverse_tree.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amabrouk <amabrouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 07:51:20 by amabrouk          #+#    #+#             */
/*   Updated: 2024/06/06 15:56:11 by amabrouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void sigint_handler_cmd(int sig)
{
    printf("\n");
    rl_on_new_line();
    rl_replace_line("", 0);
}

char *args_to_str(t_arg *args)
{
	char *result;

	result = NULL;
	while (args)
	{
		result = ft_strjoin(result, args->content, GC);
		args = args->next;
	}
	return (result);
}

t_arg *final_args(t_cmd_arg *cmd_arg)
{
	t_arg *args;
	t_arg *head;
	t_arg *current;
	char *ready_arg;

	head = NULL;
	ready_arg = NULL;
	while(cmd_arg)
	{
		args = cmd_arg->arg;
		while(args)
		{
			ready_arg = ft_strjoin(ready_arg, args->content, GC);
			args = args->next;
		}
		current = ft_lstnew_arg(NULL);
		current->content = ready_arg;
		ft_lstaddback_arg(&head, current);
		cmd_arg = cmd_arg->next;
	}
	return (head);
}

t_arg *expand_args(t_cmd_arg *cmd_arg, t_env *env)
{
	t_arg *args;
	t_cmd_arg *original;

	original = cmd_arg;
	while(cmd_arg)
	{
		args = cmd_arg->arg;
		while(args)
		{
			if(args->to_replace != NO_REPLACE)
			{
					args->content = env_expander(args->content, env);
				if(args->to_replace == REPLACE_ALL)
					args->content = star_matching(args->content);
			}
			args = args->next;
		}
		cmd_arg = cmd_arg->next;
	}
	return (final_args(original));
}

void expand_redirs(t_redir *redir, t_env *env)
{
	while (redir)
	{
		if(redir->to_replace != NO_REPLACE)
		{
			redir->redir_string = env_expander(redir->redir_string, env);
			if(redir->to_replace == REPLACE_ALL)
				redir->redir_string = star_matching(redir->redir_string);
		}
		redir = redir->next;
	}
}

void expand_node(t_treenode *root, t_env *env)
{
	t_arg *command;

	command = root->command;
	if(command)
	{
		while(command)
		{
			if(command->to_replace != NO_REPLACE)
				command->content = env_expander(command->content, env);
			if(command->to_replace == REPLACE_ALL)
				command->content = star_matching(command->content);
			command = command->next;
		}
		root->content = args_to_str(root->command);
	}
	root->args = expand_args(root->cmd_arg, env);
	expand_redirs(root->before_redir, env);
	expand_redirs(root->after_redir, env);
}


int	traverse_tree(t_treenode *root, t_data *data, t_env **env)
{
	int		save_in;
	int		save_out;

	save_in = dup(0);
	save_out = dup(1);
	signal(SIGINT, sigint_handler_cmd);
	if (!root)
		return (0);
	if(root->token != AND  && root->token != OR && root->token != PIPE_LINE)
		expand_node(root, *env);
	if (root->before_redir)
	{
		handle_red(root->before_redir);
		printf("before\n");
	}
	if (root->after_redir)
	{
		handle_red(root->after_redir);
		printf("after\n");
	}
	if (root->token == COMMAND)
		execute_command(root, env, data);
	else if (root->token == PIPE_LINE)
		pipeline(root, data, env);
	else if (root->token == AND)
	{
		if (traverse_tree(root->left, data, env) == 0 || data->status == 0)
			traverse_tree(root->right, data, env);
	}
	else if (root->token == OR)
	{
		if (traverse_tree(root->left, data, env) == 0 || data->status != 0)
			traverse_tree(root->right, data, env);
	}
	else
		fprintf(stderr, "ERROR: %i\n", root->token);
	dup2(save_in, 0);
	dup2(save_out, 1);
	close(save_in);
	close(save_out);
	return 1;
}
