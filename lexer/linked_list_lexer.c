#include "../minishell.h"

t_lex	*ft_lstnew_lex(char *content, int token, int len)
{
	t_lex	*new_node;

	new_node = malloc(sizeof(t_lex));
	if (!new_node)
		return (NULL);
	new_node->content = content;
	new_node->len = len;
	new_node->token = token;
	new_node->state = -1;
	new_node->next = NULL;
	new_node->prev = NULL;
	return (new_node);
}


t_lex	*ft_lstlast_lex(t_lex *lst)
{
	t_lex	*temp;

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

t_lex	*ft_lstfirst_lex(t_lex *lst)
{
	t_lex	*temp;

	temp = lst;
	while (temp)
	{
		if (!temp->prev)
			return (temp);
		temp = temp->prev;
	}
	return (lst);
}

int counter(t_lex *current, char c)
{
	int count;

	count = 0;
	while(current)
	{
		if(current->len == 1 && current->content[0] == c)
			count++;
		current = current->prev;
	}
	return (count);
}


void	ft_lstadd_back_lex(t_lex **lst, t_lex *new)
{
	t_lex	*last_lex;

	if (!lst)
		return ;
	if (!*lst)
	{
		*lst = new;
		new->state = GENERAL;
		return ;
	}
	last_lex = ft_lstlast_lex(*lst);
	last_lex->next = new;
	new->prev = last_lex;
	if(new->prev && (new->prev->token == DOUBLE_QUOTE || new->prev->state == IN_DQUOTE) && (counter(new, '"') % 2)  && new->token != DOUBLE_QUOTE)
		new->state = IN_DQUOTE;
	else if (new->prev && (new->prev->token == QUOTE || new->prev->state == IN_QUOTE) && (counter(new, '\'') % 2) && new->token != QUOTE)
		new->state = IN_QUOTE;
	else
		new->state = GENERAL;
}

const char* tokenToString(token t)
{
    switch (t) 
	{
        case WORD: return "WORD";
        case WHITE_SPACE: return "WHITE_SPACE";
        case NEW_LINE: return "NEW_LINE";
        case QUOTE: return "QUOTE";
        case DOUBLE_QUOTE: return "DOUBLE_QUOTE";
        case ESCAPE: return "ESCAPE";
        case ENV: return "ENV";
        case PIPE_LINE: return "PIPE_LINE";
        case REDIR_IN: return "REDIR_IN";
        case REDIR_OUT: return "REDIR_OUT";
        case HERE_DOC: return "HERE_DOC";
        case DREDIR_OUT: return "DREDIR_OUT";
        default: return "UNKNOWN_TOKEN";
    }
}

const char* stateToString(state s)
{
    switch (s) {
        case IN_DQUOTE: return "IN_DQUOTE";
        case IN_QUOTE: return "IN_QUOTE";
        case GENERAL: return "GENERAL";
        default: return "UNKNOWN_STATE";
    }
}


void print_lex(const t_lex *lex) {

    printf("| %-10s | %-15s | %-10d | %-15s |\n", lex->content, tokenToString(lex->token), lex->len, stateToString(lex->state));
    printf("| %-10s | %-15s | %-10s | %-15s |\n", "", "", "", "");
    printf("|------------|-----------------|------------|-----------------|\n");
}

void ft_lstiter_lex(t_lex *lex)
{
    printf("| %-10s | %-15s | %-10s | %-15s |\n", "Content", "Token", "Length", "State");
    printf("|------------|-----------------|------------|-----------------|\n");
	while(lex)
	{
		print_lex(lex);
		lex = lex->next;
	}
}