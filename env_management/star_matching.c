#include "../minishell.h"


t_env *get_star() // HANDLE STAR MATCHING
{
	DIR *current_wd;
    struct dirent *read;
    char *ret;
	int last_type;
	t_env *current;
	t_env *head;

	ret = NULL;
	current = NULL;
	head = NULL;
	current_wd = opendir(".");
	if(!current_wd)
		return NULL; 
	read = readdir(current_wd);
	while (read != NULL)
	{
		if (read->d_type == DT_REG || read->d_type == DT_DIR && read->d_name[0] != '.')
		{
			current = ft_lstnew_env(read->d_name);
			ft_lstadd_back_env(&head, current);
			last_type = read->d_type;
		}
		read = readdir(current_wd);
	}
	closedir(current_wd);
	return (head);
}

char *normalize_pattern(char *pattern)
{
    char *normalized;
    char *start;
    bool star_found = false;
    int pattern_len = ft_strlen(pattern);
    int i = 0;
    int j = 0;

    normalized = smart_malloc(pattern_len + 1);
    start = normalized;
    
    while (pattern[i])
    {
        if (pattern[i] == '*')
        {
            if (!star_found)
            {
                normalized[j] = pattern[i];
                j++;
                star_found = true;
            }
        }
        else
        {
            normalized[j] = pattern[i];
            j++;
            star_found = false;
        }
        i++;
    }
    normalized[j] = '\0';
    return start;
}


bool match_pattern(char *str, char *pattern)
{
    char *star_ptr = NULL;
    char *star_str = NULL;
    int str_index = 0;
    int pattern_index = 0;

    while (str[str_index])
    {
        if (pattern[pattern_index] == '*')
        {
            star_ptr = &pattern[pattern_index]; // *
            star_str = &str[str_index]; // m
            pattern_index++;
        }
        else if (pattern[pattern_index] == str[str_index])
        {
            pattern_index++;
            str_index++;
        }
        else if (star_ptr)
        {
            pattern_index = star_ptr - pattern + 1;
            str_index = star_str - str + 1;
            star_str++;
        }
        else
            return false;
    }

    while (pattern[pattern_index] == '*')
        pattern_index++;
    return !pattern[pattern_index];
}

bool is_star(char *to_match)
{
    int i;

    i = 0;
    while(to_match[i])
    {
        if(to_match[i] == '*')
            return (true);
        i++;
    }
    return (false);
}

char *star_matching(char *to_match)
{
    char *temp;
    char *result;
    t_env *current_star;
    t_env *head;
    char *normal;

    head = NULL;
    result = NULL;
    current_star = get_star();
    if (!current_star)
    {
        // ft_putstr_fd(2, "opendir: can't opendir()");
        return (to_match);
    }
    normal = normalize_pattern(to_match);
    while (current_star)
    {
        if (current_star->value)
        {
            if (match_pattern(current_star->value, normal))
            {
                t_env *current = ft_lstnew_env(current_star->value);
                if (!current)
                    return NULL;
                ft_lstadd_back_env(&head, current);
            }
        }
        current_star = current_star->next;
    }
    while (head)
    {
        temp = head->value;
        if(head->next)
            temp = ft_strjoin(head->value, " ", GC);
        if (!result)
            result = temp;
        else
            result = ft_strjoin(result, temp, GC);
        head = head->next;
    }
    if(!result)
        return to_match;
    return (result);
}