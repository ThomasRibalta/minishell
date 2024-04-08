#include "../../header/minishell.h"

void export_var(char ***env, const char *name, const char *value) 
{
    char **current;
    int num_entries;

    current = *env;
    while (*current) 
    {
        if (ft_strncmp(*current, name, ft_strlen(name)) == 0) 
        {
            free(*current);
            *current = malloc(ft_strlen(name) + ft_strlen(value) + 2);
            ft_strcpy(*current, name);
            ft_strcpy(*current + ft_strlen(name), "=");
            ft_strcpy(*current + ft_strlen(name) + 1, value);
            return;
        }
        current++;
    }
    num_entries = current - *env;
    *env = realloc(*env, (num_entries + 2) * sizeof(char *)); // penser a le recoder
    (*env)[num_entries] = malloc(ft_strlen(name) + ft_strlen(value) + 2);
    ft_strcpy((*env)[num_entries], name);
    ft_strcpy((*env)[num_entries] + ft_strlen(name), "=");
    ft_strcpy((*env)[num_entries] + ft_strlen(name) + 1, value);
    (*env)[num_entries + 1] = NULL;
}