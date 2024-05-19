#include "../minishell.h"


char **copy_args(char **args)
{
	char **result;
	int c;

	c = 0;
	while(args[c])
		c++;
	result = malloc((c + 1) * sizeof(char *));
	c = 0;
	while(args[c])
	{
		result[c] = ft_strdup(args[c]);
		c++;
	}
	result[c] = NULL;
	return (result);
}

t_treenode		*new_treenode(t_middle *middled)
{
	t_treenode		*new_node;

	new_node = malloc(sizeof(t_treenode));
	if (!new_node)
		return (NULL);
	new_node->token = middled->token;
	new_node->content = ft_strdup(middled->content);
	new_node->args = NULL;
	if(middled->args)
		new_node->args = copy_args(middled->args);
	new_node->after_redir = NULL;
	new_node->before_redir = NULL;
	// new_node->before_redir.is_redirected = false;
	// new_node->before_redir.token = NONE;
	// new_node->before_redir.redir_string = NULL;
	// new_node->after_redir.is_redirected = false;
	// new_node->after_redir.token = NONE;
	// new_node->after_redir.redir_string = NULL;
	new_node->left = NULL;
	new_node->right = NULL;
	return (new_node);
}


t_redir *ft_lstnew_redir(t_middle *middled)
{
	t_redir *new_node;

	new_node = malloc(sizeof(t_redir));
	if(!new_node)
		return (NULL);
	new_node->token = (middled)->token;
	new_node->redir_string = ft_strdup((middled)->redir_string);
	new_node->next = NULL;
	return (new_node);
}

t_redir *ft_lstlast_redir(t_redir *first)
{
	while(first)
	{
		if(!first->next)
			return (first);
		first = first->next;
	}
	return (NULL);
}

void ft_lstaddback_redir(t_redir **head, t_redir *new)
{
	t_redir *last;
	if(!head)
		return;
	if(!*head)
	{
		*head = new;
		return ;
	}
	last = ft_lstlast_redir(*head);
	last->next = new;
}

void ft_lstiter_redir(t_redir *first)
{
	while(first)
	{
		printf("%s %s ", tokenToString(first->token), first->redir_string);
		first = first->next;
	}
}

void change_root_to(t_treenode **from_node, t_treenode *to_node)
{
	t_treenode *new_left;

	to_node->left = *from_node;
	*from_node = to_node;
}