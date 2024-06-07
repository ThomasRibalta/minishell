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

int update_existing_entry(char **current, const char *name, const char *value)
{
    if (ft_strncmp(*current, name, ft_strlen(name)) == 0)
    {
        free(*current);
        *current = malloc(ft_strlen(name) + ft_strlen(value) + 2);
        if (!(*current))
            return 1;
        ft_strcpy(*current, name);
        ft_strcpy(*current + ft_strlen(name), "=");
        ft_strcpy(*current + ft_strlen(name) + 1, value);
        return 0;
    }
    return -1;
}

char **duplicate_env(char **env)
{
    char **new_env;
    int num_entries; 
    int i;
    
    new_env = NULL;
    num_entries = 0;
    i = -1;
    while (env[num_entries])
        num_entries++;
    new_env = malloc((num_entries + 1) * sizeof(char *));
    if (!new_env)
        return NULL;
    while (++i < num_entries)
    {
        new_env[i] = ft_strdup(env[i]);
        if (!new_env[i])
        {
            while (--i >= 0)
                free(new_env[i]);
            free(new_env);
            return NULL;
        }
    }
    new_env[num_entries] = NULL;
    return new_env;
}

char **init_new_env(char ***env, int num_entries, const char *name, const char *value)
{
    char **new_env;

    new_env = malloc((num_entries + 2) * sizeof(char *));
    if (!new_env)
        return (NULL);
    new_env = duplicate_env(*env);
    if (!new_env)
    {
        free(new_env);
        return (NULL);
    }
    new_env[num_entries] = malloc(ft_strlen(name) + ft_strlen(value) + 2);
    if (!new_env[num_entries])
    {
        free(new_env);
        return (NULL);
    }
    return new_env;
}