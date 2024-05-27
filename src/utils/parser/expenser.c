#include "../../header/minishell.h"


void processBinaryTree(ASTNode* node, void (*processStr)(char**, char**), char **env) {
    if (node == NULL) return;

    // Process the left subtree first
    processBinaryTree(node->left, processStr, env);

    // Process the command value only for COMMAND nodes
    if (node->type == NODE_COMMAND) {
        processStr(&node->value, env);
    }

    // Process the right subtree last
    processBinaryTree(node->right, processStr, env);
}

void expandCommandTrees(StartNode* startNode, void (*processStr)(char**, char**), char **env) {
    if (!startNode->hasLogical) {
        // If there are no logical operators, process the entire tree under the HOLDER node.
        processBinaryTree(startNode->children[0]->left, processStr, env);
    } else {
        // If logical operators are present, process each tree attached to the logical nodes.
        for (int i = 0; i < startNode->childCount; i++) {
            if (startNode->children[i]->left) {
                processBinaryTree(startNode->children[i]->left, processStr, env);
            }
            if (i == 0 && startNode->children[i]->right) {
                // For the first logical node, also process the right subtree
                processBinaryTree(startNode->children[i]->right, processStr, env);
            }
        }
    }
}


void printCommandNode(char **str) {
	printf("%s|\n", *str);
}

void is_last_command_btree(ASTNode* node) {
    if (node == NULL) return;

	while (node->right != NULL)
	{
		node = node->right;
	}
    node->is_last_command = true;
}

void is_last_command(StartNode* startNode) {
    if (!startNode->hasLogical) {
        is_last_command_btree(startNode->children[0]->left);
    } else {
        for (int i = 0; i < startNode->childCount; i++) {
            is_last_command_btree(startNode->children[i]->left);
            if (i == 0 && startNode->children[i]->right) {
                is_last_command_btree(startNode->children[i]->right);
            }
        }
    }
}

// char *get_word(char *str)
// {
//     int i = 0;
//     while (str[i] && (str[i] != ' ' && str[i] != '$'))
//         i++;
//     char *word = malloc(i + 1);
//     if (!word)
//         return (NULL);
//     i = 0;
//     while (str[i] && (str[i] != ' ' && str[i] != '$'))
//     {
//         word[i] = str[i];
//         i++;
//     }
//     word[i] = '\0';
//     return (word);
// }

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

char *get_env_value(char *word, char **env , int *exit_status)
{
    int i = 0;
    while (env[i])
    {
        if (word[0] == '?')
        {
            char *value = ft_itoa(MY_WEXITSTATUS(*exit_status));
            return (value);
        }
        if (ft_strncmp(word, env[i], ft_strlen(word)) == 0)
            return (ft_substr(env[i], ft_strlen(word) + 1, ft_strlen(env[i]) - ft_strlen(word) - 1));
        i++;
    }
    return (NULL);
}

// utiliser fonction thomas pour les quotes externe
void replaceEnvVars(char **str, char **env, int *exit_status)
{
    int i = 0;
    int j = 0;
    char *tmp = *str;
    while (tmp[i])
    {
        if (tmp[i] == '$' && in_special_zone(tmp, i) != 2)
        {
            j = 1;
            char *word = ft_substr(tmp + i + 1, 0, j);
            while ((!in_env(ft_strjoin(word, "="), env) && tmp[i + j]) || word[0] != '?')
            {
                word = ft_substr(tmp + i + 1, 0, j);
                j++;
            }
            if (in_env(word, env) || word[0] == '?')
            {
                char *value = get_env_value(word, env, exit_status);
                if (value)
                {
                    char *new_str = ft_strjoin(ft_substr(tmp, 0, i), value);
                    char *tmp2 = ft_strjoin(new_str, tmp + i + ft_strlen(word) + 1);
                    tmp = tmp2;
                    i += ft_strlen(value) - 1;
                }
            }
            else
                i++;
        }
        i++;
    }
    *str = tmp;
}

void expenser(StartNode* startNode)
{
	//expandCommandTrees(startNode, replaceEnvVars, env);
	is_last_command(startNode);
}
