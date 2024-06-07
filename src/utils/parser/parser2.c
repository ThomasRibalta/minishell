#include "../../header/minishell.h"


void type_command_or_parent(Token **cT, ASTNode **currentCommand, ASTNode **root, ASTNode **last)
{
    if ((*cT)->type == TOKEN_COMMAND)
        *currentCommand = createASTNode(NODE_COMMAND, (*cT)->value);
    else
        *currentCommand = createASTNode(NODE_PARENTHESE, (*cT)->value);
    if (*root == NULL)
        *root = *currentCommand;
    else
    {
        *last = *root;
        while ((*last)->right)
            *last = (*last)->right;
        (*last)->right = *currentCommand;
    }
}


void init_empty_cmd(Parsertmp *parsertmp)
{
    parsertmp->currentCommand = createASTNode(NODE_EMPTY_COMMAND, NULL);
    (parsertmp->currentCommand)->inputs = parsertmp->tempInputs;
    (parsertmp->currentCommand)->outputs = parsertmp->tempOutputs;
    
    if (parsertmp->root == NULL)
        parsertmp->root = parsertmp->currentCommand;
    else
    {
        parsertmp->last = parsertmp->root;
        while ((parsertmp->last)->right)
            parsertmp->last = (parsertmp->last)->right;
        (parsertmp->last)->right = parsertmp->currentCommand;
    }
}

void parser_redirection(Redirection **tempInputs, Redirection **tempOutputs, Token **cT)
{
    Redirection* newRedir;
    
    newRedir = createRedirection((*cT)->value, ((*cT)->type == TOKEN_APPEND || (*cT)->type == TOKEN_HEREDOC));       
    if ((*cT)->type == TOKEN_IN || (*cT)->type == TOKEN_HEREDOC)
        addRedirection(tempInputs, newRedir);
    else if ((*cT)->type == TOKEN_OUT || (*cT)->type == TOKEN_APPEND)
        addRedirection(tempOutputs, newRedir);
}


void handlePipeOrLogicalOperator(Token** cT, Parsertmp *parsertmp)
{
    ASTNode* pipeNode;

    if (!(parsertmp->currentCommand) && (parsertmp->tempInputs || parsertmp->tempOutputs))
        init_empty_cmd(parsertmp);
    else
    {
        (parsertmp->currentCommand)->inputs = parsertmp->tempInputs;
        (parsertmp->currentCommand)->outputs = parsertmp->tempOutputs;
    }
    if ((*cT)->type == TOKEN_PIPE)
    {
        pipeNode = createASTNode(NODE_PIPE, "|");
        pipeNode->left = parsertmp->root;
        parsertmp->root = pipeNode;
        parsertmp->currentCommand = NULL;
    }
    else
    {
        parsertmp->tempInputs = parsertmp->tempOutputs = NULL;
    }
}
