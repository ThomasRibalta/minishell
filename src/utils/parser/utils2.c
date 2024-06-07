#include "../../header/minishell.h"

void addLogicalNodeToStartNode(StartNode* startNode, Token* tokens)
{
    int index;
    Token* cT;
    NodeType type;

    if (startNode->hasLogical)
    {
        index = 0;
        cT = tokens;
        while (cT != NULL && index < startNode->childCount)
        {
            if (cT->type == TOKEN_LOGICAL_AND || cT->type == TOKEN_LOGICAL_OR)
            {
                if (cT->type == TOKEN_LOGICAL_AND)
                    type = NODE_LOGICAL_AND;
                else
                    type = NODE_LOGICAL_OR;
                startNode->children[index] = createLogicalNode(type);
                index++;
            }
            cT = cT->next;
        }
    } 
    else
        startNode->children[0] = createLogicalNode(NODE_LOGICAL_HOLDER);
}

int in_env(char *word, char **env)
{
    int i = 0;
    while (env[i])
    {
        if (ft_strncmp(word, env[i], ft_strlen(word)) == 0)
            return (1);
        i++;
    }
    return (0);
}

char *get_env_value(char *word, char **env)
{
    int i;
    
    i = 0;
    while (env[i])
    {
        if (ft_strncmp(word, env[i], ft_strlen(word)) == 0)
            return (ft_substr(env[i], ft_strlen(word) + 1, ft_strlen(env[i]) - ft_strlen(word) - 1));
        i++; 
    }
    return (NULL);
}

char *get_word_env(char *tmp, int i, int *j, char **env)
{
    char *word;

    word = ft_substr(tmp + i + 1, 0, *j);
    while ((in_env(ft_strjoin(word, ft_strdup("=")), env) != 1 &&
    tmp[i + *j] && tmp[i + *j] != ' ' && tmp[i + *j] != '$'))
    {
        free(word);
        word = ft_substr(tmp + i + 1, 0, *j);
        (*j)++;
    }
    return (word);

}
