#include "../../header/minishell.h"

int countLogicalNodes(Token* tokens)
{
    int count;
    Token* current;

    count = 0;
    current = tokens;
    while (current != NULL)
    {
        if (current->type == TOKEN_LOGICAL_AND || current->type == TOKEN_LOGICAL_OR)
            count++;
        current = current->next;
    }
    return count;
}

StartNode* createAndSetupStartNode(Token* tokens)
{
    int logicalNodeCount;
    StartNode* startNode;

    startNode = (StartNode*)malloc(sizeof(StartNode));
    if (!startNode)
        exit(EXIT_FAILURE);
    startNode->hasLogical = false;
    startNode->childCount = 0;
    startNode->children = NULL;
    logicalNodeCount = countLogicalNodes(tokens);
    if (logicalNodeCount > 0)
    {
        startNode->hasLogical = true;
        startNode->childCount = logicalNodeCount;
    }
    else
        startNode->childCount = 1;
	startNode->children = (LogicalNode**)malloc(sizeof(LogicalNode*) * startNode->childCount);
	if (!startNode->children)
    {
		perror("Failed to allocate memory for logical node pointers in StartNode");
		free(startNode);
		exit(EXIT_FAILURE);
	}
    return startNode;
}

ASTNode* createASTNode(NodeType type, char* value)
{
    ASTNode* node;

    node = (ASTNode*)malloc(sizeof(ASTNode));
    if (!node)
    {
        perror("Failed to allocate memory for ASTNode");
        exit(EXIT_FAILURE);
    }
    node->type = type;
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    node->inputs = NULL;
    node->outputs = NULL;
    node->appends = NULL;
	node->here_doc = NULL;
	node->builtIn = false;
	node->is_last_command = false;
    return node;
}

LogicalNode* createLogicalNode(NodeType type)
{
    LogicalNode* node;

    node = (LogicalNode*)malloc(sizeof(LogicalNode));
    if (!node)
    {
        perror("Failed to allocate memory for LogicalNode");
        exit(EXIT_FAILURE);
    }
    node->type = type;
    node->left = NULL;
    node->right = NULL;
    return node;
}
