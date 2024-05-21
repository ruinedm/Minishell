#include "../minishell.h"



t_middle *middle_input_checker(t_middle *middled)
{
	t_middle *next;

	while(middled)
	{
		if(middled->token == OPEN_PARANTHESE)
		{
			next = middled->next;
			if(!next)
				return (middled);
			else if(next->token == CLOSE_PARANTHESE)
			{
				fprintf(stderr, "Hi\n");
				return (next);
			}
		}
		middled = next;
	}
	return (NULL);
}