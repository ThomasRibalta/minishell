#include "../../header/minishell.h"

int error_name(const char *name) 
{
    int i;

    i = 0;
    if (!ft_isalpha(name[0]) && name[0] != '_') 
    {
        printf("export: `%s` not a valid identifier\n", name);
        return (1);
    }
    while (name[i]) 
    {
        if (!ft_isalnum(name[i]) && name[i] != '_') 
        {
            printf("export: `%s` not a valid identifier\n", name);
            return (1);
        }
        i++;
    }
    return (0);
}

int export_var2(char ***env, const char *name, const char *value) 
{
    char **current;
    int num_entries;

    current = *env;
    if (error_name(name) == 1)
        return (1);
    while (*current) 
    {
        if (ft_strncmp(*current, name, ft_strlen(name)) == 0) 
        {
            free(*current);
            *current = malloc(ft_strlen(name) + ft_strlen(value) + 2);
            if (*current == NULL) 
            {
                return (1);
            }
            ft_strcpy(*current, name);
            ft_strcpy(*current + ft_strlen(name), "=");
            ft_strcpy(*current + ft_strlen(name) + 1, value);
            return (1);
        }
        current++;
    }
    num_entries = current - *env;
    char **new_env = realloc(*env, (num_entries + 2) * sizeof(char *));
    if (new_env == NULL) 
        return (1);
    *env = new_env;
    (*env)[num_entries] = malloc(ft_strlen(name) + ft_strlen(value) + 2);
    if ((*env)[num_entries] == NULL) 
        return (1);
    ft_strcpy((*env)[num_entries], name);
    ft_strcpy((*env)[num_entries] + ft_strlen(name), "=");
    ft_strcpy((*env)[num_entries] + ft_strlen(name) + 1, value);
    (*env)[num_entries + 1] = NULL;
    return (0);
}

int export_var(char ***env,char ***export, char **tab) 
{
    int i;
    int status;

    i = 0;
    status = 0;
    while(tab[i])
    {
        if (!ft_strchr(tab[i], '='))
        {
            status = export_var2(export, tab[i], "");
        }
        else
        {
            status = export_var2(env, ft_strndup(tab[i], ft_strchr(tab[i], '=') - tab[i]), ft_strchr(tab[i], '=') + 1);
            status = export_var2(export, ft_strndup(tab[i], ft_strchr(tab[i], '=') - tab[i]), ft_strchr(tab[i], '=') + 1);
        }
        i++;
    }
    return (status);
}