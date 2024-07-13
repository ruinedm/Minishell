#include "../minishell.h"


t_env *get_star(int mode) // HANDLE STAR MATCHING
{
	DIR *current_wd;
    struct dirent *read;
	t_env *current;
	t_env *head;
	bool condition;

	current = NULL;
	head = NULL;
	current_wd = opendir(".");
	if(!current_wd)
		return NULL; 
	read = readdir(current_wd);
	while (read != NULL)
	{
		if(mode == SEEN && read->d_name[0] == '.')
			condition = false;
		else
			condition = true;
		if ((read->d_type == DT_REG || read->d_type == DT_DIR) && condition)
		{
			current = ft_lstnew_env(read->d_name, GC);
			ft_lstadd_back_env(&head, current);
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


t_env *star_matching(char *to_match)
{
    t_env *current_star;
    t_env *head;
    t_env *current;
    char *normal;

    head = NULL;
	if(to_match[0] == '.')
    	current_star = get_star(HIDDEN);
	else
		current_star = get_star(SEEN);
    if (!current_star)
		return (NULL);
    normal = normalize_pattern(to_match);
    while (current_star)
    {
        if (current_star->value && match_pattern(current_star->value, normal))
        {
			current = ft_lstnew_env(current_star->value, GC);
			ft_lstadd_back_env(&head, current);
        }
        current_star = current_star->next;
	}
	return (sort_env_list(head), head);
}

t_arg *arg_star_matching(char *to_match)
{
	t_arg *head;
	t_arg *current;
	t_env *star_res;

	head = NULL;
	star_res = star_matching(to_match);
	while(star_res)
	{
		current = ft_lstnew_arg(NULL);
		current->content = ft_strdup(star_res->value, GC);
		ft_lstaddback_arg(&head, current);
		star_res = star_res->next;
	}
	return (head);
}


t_cmd_arg *cmd_arg_star_matching(char *to_match)
{
	t_cmd_arg *head_arg;
	t_cmd_arg *current_arg;
	t_arg *arg;
	t_env *env_star;

	head_arg = NULL;
	env_star = star_matching(to_match);
	while(env_star)
	{
		arg = ft_lstnew_arg(NULL);
		arg->content = ft_strdup(env_star->value, GC);
		current_arg = ft_lstnew_cmd_arg(arg);
		ft_lstaddback_cmd_arg(&head_arg, current_arg);
		env_star = env_star->next;
	}
	return (head_arg);
}
