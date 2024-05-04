#include "../../header/minishell.h"

void export_var2(char ***env, const char *name, const char *value) 
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
            if (*current == NULL) 
            {
                // Gérer l'erreur d'allocation de mémoire ici
                return;
            }
            ft_strcpy(*current, name);
            ft_strcpy(*current + ft_strlen(name), "=");
            ft_strcpy(*current + ft_strlen(name) + 1, value);
            return;
        }
        current++;
    }
    num_entries = current - *env;
    char **new_env = realloc(*env, (num_entries + 2) * sizeof(char *));
    if (new_env == NULL) 
    {
        // Gérer l'erreur d'allocation de mémoire ici
        return;
    }
    *env = new_env;
    (*env)[num_entries] = malloc(ft_strlen(name) + ft_strlen(value) + 2);
    if ((*env)[num_entries] == NULL) 
    {
        // Gérer l'erreur d'allocation de mémoire ici
        return;
    }
    ft_strcpy((*env)[num_entries], name);
    ft_strcpy((*env)[num_entries] + ft_strlen(name), "=");
    ft_strcpy((*env)[num_entries] + ft_strlen(name) + 1, value);
    (*env)[num_entries + 1] = NULL;
}

void export_var(char ***env, char **tab) 
{
    int i;

    i = 0;
    while(tab[i])
    {
        printf("tab[%d] = %s\n", i, tab[i]);
        if (!ft_strchr(tab[i], '='))
        {
            export_var2(env, tab[i], "");
        }
        else
        {
            export_var2(env, ft_strndup(tab[i], ft_strchr(tab[i], '=') - tab[i]), ft_strchr(tab[i], '=') + 1);
        }
        i++;
    }
    print_env(*env);
}