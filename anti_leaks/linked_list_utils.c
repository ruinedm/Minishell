#include "cgc.h"
#include <stdio.h>
t_node	*ft_lstnew(void *ptr_to_store)
{
	t_node	*new_node;

	new_node = malloc(sizeof(t_node));
	if (!new_node)
		return (NULL);
	new_node->ptr_to_free = ptr_to_store;
	new_node->next = NULL;
	new_node->prev = NULL;
	return (new_node);
}
t_node	*ft_lstlast(t_node *lst)
{
	t_node	*temp;

	if (lst == NULL)
		return (NULL);
	temp = lst;
	while (temp)
	{
		if (!temp->next)
			return (temp);
		temp = temp->next;
	}
	return (lst);
}

void	ft_lstadd_back(t_node *lst, t_node *new_node)
{
	t_node *last;

	if (!lst)
		return ;
	last = ft_lstlast(lst);
	last->next = new_node;
	new_node->prev = last;
}

void ft_lstclear(t_node **lst)
{
    t_node *tmp;

    if (lst == NULL)
        return;
    
    while (*lst != NULL)
    {
        tmp = (*lst)->next;
        free((*lst)->ptr_to_free);
        free(*lst);
        *lst = tmp;
    }
}

t_fd	*ft_lstnew_fd(int fd_to_store)
{
	t_fd	*new_node;

	new_node = smart_malloc(sizeof(t_fd));
	new_node->fd = fd_to_store;
	new_node->next = NULL;
	new_node->prev = NULL;
	return (new_node);
}

t_fd	*ft_lstlast_fd(t_fd *lst)
{
	t_fd	*temp;

	if (lst == NULL)
		return (NULL);
	temp = lst;
	while (temp)
	{
		if (!temp->next)
			return (temp);
		temp = temp->next;
	}
	return (lst);
}

void	ft_lstadd_back_fd(t_fd *lst, t_fd *new_node)
{
	t_fd *last;

	if (!lst)
		return ;
	last = ft_lstlast_fd(lst);
	last->next = new_node;
	new_node->prev = last;
}