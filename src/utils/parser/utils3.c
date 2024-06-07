#include "../../header/minishell.h"

char *replaceSubstring(char *str, int start, int end, char *replacement)
{
    char *new_str;
    char *tmp;
    
    new_str = ft_strjoin(ft_substr(str, 0, start), replacement);
    tmp = ft_strjoin(new_str, str + end);
    free(new_str);
    return tmp;
}

void replace_variable(char **tmp, int *i, char **env, int *exit_status)
{
    char *word;
    char *value;
    char *new_str;
    int j;

    j = 1;
    word = get_word_env(*tmp, *i, &j, env);
    if (in_env(word, env) || word[0] == '?')
    {
        if (word[0] == '?')
            value = ft_itoa(*exit_status);
        else
            value = get_env_value(word, env);
        free(word);
        if (value)
            *tmp = replaceSubstring(*tmp, *i, *i + j, value);
    }
    else
    {
        new_str = ft_strjoin(ft_substr(*tmp, 0, *i), (*tmp) + *i + j);
        *tmp = new_str;
        (*i)--;
    }
}
void replaceEnvVars(char **str, char **env, int *exit_status)
{
    int i;
    char *tmp;

    i = -1;
    tmp = *str;
    while (tmp[++i])
    {
        if (tmp[i] == '$' && in_special_zone(tmp, i) != 2 && tmp[i + 1])
        {
            replace_variable(&tmp, &i, env, exit_status);
        }
    }
    *str = tmp;
}

void  edit_shlvl(char **env)
{
    while (*env) 
    {
        if (ft_strncmp(*env, "SHLVL=", 6) == 0) 
        {
            ft_strcpy(*env + 6,ft_itoa(ft_atoi(*env + 6) + 1));
            break;
        }
        env++;
    }
}