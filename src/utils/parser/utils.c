#include "../../header/minishell.h"

const char* getNodeTypeString(NodeType type)
{
    if (type == NODE_COMMAND)
        return "COMMAND";
    else if (type == NODE_EMPTY_COMMAND)
        return "EMPTY COMMAND";
    else if (type == NODE_PARENTHESE)
        return "PARENTHESE";
    else if (type == NODE_LOGICAL_AND)
        return "LOGICAL AND";
    else if (type == NODE_LOGICAL_OR)
        return "LOGICAL OR";
    else if (type == NODE_PIPE)
        return "PIPE";
    else if (type == NODE_LOGICAL_HOLDER)
        return "LOGICAL HOLDER";
    else
        return "UNKNOWN";
}

void init_parsertmp(Parsertmp *parsertmp)
{
    parsertmp->root = NULL;
    parsertmp->last = NULL;
    parsertmp->currentCommand = NULL;
    parsertmp->tempInputs = NULL;
    parsertmp->tempOutputs = NULL;
}

ASTNode* buildCommandPipeTree(Token** cT)
{
    Parsertmp parsertmp;

    init_parsertmp(&parsertmp);
    while (*cT)
    {
        if ((*cT)->type == TOKEN_COMMAND || (*cT)->type == TOKEN_PAREN)
            type_command_or_parent(cT, &parsertmp.currentCommand, &parsertmp.root, &parsertmp.last);
        else if ((*cT)->type == TOKEN_PIPE || (*cT)->type == TOKEN_LOGICAL_AND || (*cT)->type == TOKEN_LOGICAL_OR)
        {
            handlePipeOrLogicalOperator(cT,  &parsertmp);
            if ((*cT)->type != TOKEN_PIPE)
                return (parsertmp.root);
        }
        else if ((*cT)->type == TOKEN_IN || (*cT)->type == TOKEN_OUT || (*cT)->type == TOKEN_APPEND || (*cT)->type == TOKEN_HEREDOC)
            parser_redirection(&parsertmp.tempInputs, &parsertmp.tempOutputs, cT);
        *cT = (*cT)->next;
    }
    if (parsertmp.currentCommand == NULL && (parsertmp.tempInputs || parsertmp.tempOutputs))
        init_empty_cmd(&parsertmp);
    else if (parsertmp.currentCommand)
    {
        parsertmp.currentCommand->inputs = parsertmp.tempInputs;
        parsertmp.currentCommand->outputs = parsertmp.tempOutputs;
    }
    return (parsertmp.root);
}

void processTokens(StartNode* startNode, Token* tokens)
{
    int count;
    
    count = -1;
    while (tokens != NULL)
    {
        if (tokens->type == TOKEN_LOGICAL_AND || tokens->type == TOKEN_LOGICAL_OR)
        {
            count++;
            tokens = tokens->next;
        }
        else
        {
            if (count == -1)
                startNode->children[0]->left = buildCommandPipeTree(&tokens);
            else if (count == 0)
                startNode->children[0]->right = buildCommandPipeTree(&tokens);
            else
                startNode->children[count]->left = buildCommandPipeTree(&tokens);
        }
    }
}