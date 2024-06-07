#include "../../header/minishell.h"

Redirection* createRedirection(char* filename, int caracteristic)
{
    Redirection* redir;

    redir = (Redirection*)malloc(sizeof(Redirection));
    if (!redir)
    {
        perror("Failed to allocate memory for Redirection");
        exit(EXIT_FAILURE);
    }
    redir->filename = filename;
    redir->caracteristic = caracteristic;
    redir->next = NULL;
    return redir;
}

void addRedirection(Redirection** list, Redirection* redir)
{
    Redirection* current;

    if (*list == NULL)
        *list = redir;
    else
    {
        current = *list;
        while (current->next != NULL)
            current = current->next;
        current->next = redir;
    }
}

void generateAndAttachBTree(StartNode* startNode, Token* tokens)
{
    LogicalNode* holder;

    if (!startNode->hasLogical)
    {
        holder = startNode->children[0];
        holder->left = buildCommandPipeTree(&tokens);
    }
    else
    {
        processTokens(startNode, tokens);
    }
}


void parser(Token *tokens, char ***env, char ***export, int *exit_status)
{
    StartNode *startNode;
    
    startNode = createAndSetupStartNode(tokens);
    addLogicalNodeToStartNode(startNode, tokens);
    generateAndAttachBTree(startNode, tokens);
    expenser(startNode);
    executer(startNode, env, export, exit_status);

}
