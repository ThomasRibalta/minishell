 #include "../header/minishell.h"

// Function to count the number of logical nodes (&& and ||)
int countLogicalNodes(Token* tokens) {
    int count = 0;
    Token* current = tokens;
    while (current != NULL) {
        if (current->type == TOKEN_LOGICAL_AND || current->type == TOKEN_LOGICAL_OR) {
            count++;
        }
        current = current->next;
    }
    return count;
}

// Function to create a StartNode and allocate memory for logical nodes if present
StartNode* createAndSetupStartNode(Token* tokens) {
    StartNode* startNode = (StartNode*)malloc(sizeof(StartNode));
    if (!startNode) {
        perror("Failed to allocate memory for StartNode");
        exit(EXIT_FAILURE);
    }

    // Initially set the hasLogical flag to false and no children
    startNode->hasLogical = false;
    startNode->childCount = 0;
    startNode->children = NULL;

    // Count the number of logical nodes
    int logicalNodeCount = countLogicalNodes(tokens);

    // If logical nodes are present, allocate memory for the array of children
    if (logicalNodeCount > 0) {
        startNode->hasLogical = true;
        startNode->childCount = logicalNodeCount;
    } else {
        startNode->childCount = 1;
    }
	startNode->children = (LogicalNode**)malloc(sizeof(LogicalNode*) * startNode->childCount);
	if (!startNode->children) {
		perror("Failed to allocate memory for logical node pointers in StartNode");
		free(startNode);
		exit(EXIT_FAILURE);
	}
    return startNode;
}

// Function to create a logical node (&&, ||, or HOLDER)
LogicalNode* createLogicalNode(NodeType type) {
    LogicalNode* node = (LogicalNode*)malloc(sizeof(LogicalNode));
    if (!node) {
        perror("Failed to allocate memory for LogicalNode");
        exit(EXIT_FAILURE);
    }
    node->type = type;
    node->leftInput = NULL;
    node->leftOutput = NULL;
	node->leftAppend = NULL;
    node->rightInput = NULL;
    node->rightOutput = NULL;
	node->rightAppend = NULL;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// Function to add logical nodes to the StartNode's children array, or create a HOLDER node
void addLogicalNodeToStartNode(StartNode* startNode, Token* tokens) {
    if (startNode->hasLogical) {
        // Allocate logical nodes based on the tokens
        int index = 0;
        Token* currentToken = tokens;
        while (currentToken != NULL && index < startNode->childCount) {
            if (currentToken->type == TOKEN_LOGICAL_AND || currentToken->type == TOKEN_LOGICAL_OR) {
                NodeType type = (currentToken->type == TOKEN_LOGICAL_AND) ? NODE_LOGICAL_AND : NODE_LOGICAL_OR;
                startNode->children[index] = createLogicalNode(type);
                index++;
            }
            currentToken = currentToken->next;
        }
    } else {
        // No logical operators found, so create a HOLDER logical node
        startNode->children[0] = createLogicalNode(NODE_LOGICAL_HOLDER);
    }
}

// Function to get the string representation of the node type
const char* getNodeTypeString(NodeType type) {
    switch (type) {
        case NODE_COMMAND: return "COMMAND";
        case NODE_LOGICAL_AND: return "LOGICAL_AND";
        case NODE_LOGICAL_OR: return "LOGICAL_OR";
        case NODE_PIPE: return "PIPE";
        case NODE_LOGICAL_HOLDER: return "LOGICAL_HOLDER";
        default: return "UNKNOWN";
    }
}

void assignRedirections(StartNode* startNode, Token* tokens) {
    if (!startNode->hasLogical) {
        // Assign redirections to the HOLDER logical node
        LogicalNode* holder = (LogicalNode*)startNode->children[0]; // Assuming HOLDER is the first child
        for (Token* current = tokens; current != NULL; current = current->next) {
            if (current->type == TOKEN_REDIRECTION_IN) holder->leftInput = current->value;
            else if (current->type == TOKEN_REDIRECTION_OUT) holder->leftOutput = current->value;
            else if (current->type == TOKEN_REDIRECTION_APPEND) holder->leftAppend = current->value;
        }
    } else {
		LogicalNode* currentLogicalNode = NULL;
		int count = -1;

		for (Token* current = tokens; current != NULL; current = current->next) {
			// When encountering logical operators, adjust the current logical node and count accordingly
			if (current->type == TOKEN_LOGICAL_AND || current->type == TOKEN_LOGICAL_OR) {
				count++; // Move to the next logical section
				if (count > 0 && count < startNode->childCount) {
					// For subsequent logical nodes, only update currentLogicalNode beyond the first
					currentLogicalNode = (LogicalNode*)startNode->children[count];
				}
				continue; // Skip to next token after adjusting logical context
			}

			// Assign redirections based on count
			if (count == -1) { // Before the first logical operator
				// Use the left side of the first logical node (HOLDER node assumed here)
				if (current->type == TOKEN_REDIRECTION_IN) startNode->children[0]->leftInput = current->value;
				else if (current->type == TOKEN_REDIRECTION_OUT) startNode->children[0]->leftOutput = current->value;
				else if (current->type == TOKEN_REDIRECTION_APPEND) startNode->children[0]->leftAppend = current->value;
			} else if (count == 0) { // Between the first logical operator and the next
				// Use the right side of the first logical node
				if (current->type == TOKEN_REDIRECTION_IN) startNode->children[0]->rightInput = current->value;
				else if (current->type == TOKEN_REDIRECTION_OUT) startNode->children[0]->rightOutput = current->value;
				else if (current->type == TOKEN_REDIRECTION_APPEND) startNode->children[0]->rightAppend = current->value;
			} else if (currentLogicalNode != NULL) { // For all subsequent logical nodes
				// Assign redirections to the left side of the current logical node
				if (current->type == TOKEN_REDIRECTION_IN) currentLogicalNode->leftInput = current->value;
				else if (current->type == TOKEN_REDIRECTION_OUT) currentLogicalNode->leftOutput = current->value;
				else if (current->type == TOKEN_REDIRECTION_APPEND) currentLogicalNode->leftAppend = current->value;
			}
		}
    }
}
// TOKEN_PAREN,
//     TOKEN_COMMAND,
// 	TOKEN_PIPE
ASTNode* createASTNode(NodeType type, char* value) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (!node) {
        perror("Failed to allocate memory for LogicalNode");
        exit(EXIT_FAILURE);
    }
    node->type = type;
	node->value = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// void	generateAndAttachBTree(StartNode* startNode, Token* tokens)
// {
// 	if (!startNode->hasLogical)
// 	{
// 		LogicalNode* holder = (LogicalNode*)startNode->children[0];
// 		for (Token* current = tokens; current != NULL; current = current->next)
// 		{


// 		}
// 	}
// 	else
// 	{
// 		LogicalNode* currentLogicalNode = NULL;
// 		int count = -1;

// 		for (Token* current = tokens; current != NULL; current = current->next) {
// 			// When encountering logical operators, adjust the current logical node and count accordingly
// 			if (current->type == TOKEN_LOGICAL_AND || current->type == TOKEN_LOGICAL_OR) {
// 				count++; // Move to the next logical section
// 				if (count > 0 && count < startNode->childCount) {
// 					// For subsequent logical nodes, only update currentLogicalNode beyond the first
// 					currentLogicalNode = (LogicalNode*)startNode->children[count];
// 				}
// 				continue; // Skip to next token after adjusting logical context
// 			}

// 			// Assign redirections based on count
// 			if (count == -1)
// 			{

// 			}
// 			else if (count == 0)
// 			{

// 			}
// 			else if (currentLogicalNode != NULL)
// 			{

// 			}
// 		}
//     }
// }

// Function to print logical nodes stored in the StartNode, including redirection information
void printLogicalNodes(const StartNode* startNode) {
    if (startNode->hasLogical) {
        printf("StartNode has %d logical nodes:\n", startNode->childCount);
        for (int i = 0; i < startNode->childCount; ++i) {
            LogicalNode* logicalNode = (LogicalNode*)(startNode->children[i]);
            printf("Node %d: Type: %s\n", i, getNodeTypeString(logicalNode->type));
            // Print redirection information if available
            if (logicalNode->leftInput) printf("  Left Input: %s\n", logicalNode->leftInput);
            if (logicalNode->leftOutput) printf("  Left Output: %s\n", logicalNode->leftOutput);
            if (logicalNode->leftAppend) printf("  Left Append: %s\n", logicalNode->leftAppend);
            if (logicalNode->rightInput) printf("  Right Input: %s\n", logicalNode->rightInput);
            if (logicalNode->rightOutput) printf("  Right Output: %s\n", logicalNode->rightOutput);
            if (logicalNode->rightAppend) printf("  Right Append: %s\n", logicalNode->rightAppend);
        }
    } else {
        printf("StartNode has a logical holder node.\n");
        if (startNode->childCount == 1) {
            LogicalNode* holderNode = (LogicalNode*)(startNode->children[0]);
            printf("Holder Node: Type: %s\n", getNodeTypeString(holderNode->type));
            // Print redirection information for HOLDER node
            if (holderNode->leftInput) printf("  Left Input: %s\n", holderNode->leftInput);
            if (holderNode->leftOutput) printf("  Left Output: %s\n", holderNode->leftOutput);
            if (holderNode->leftAppend) printf("  Left Append: %s\n", holderNode->leftAppend);
        }
    }
}

void parser(Token *token)
{
	StartNode *startNode = createAndSetupStartNode(token);
	addLogicalNodeToStartNode(startNode, token);
	assignRedirections(startNode, token);
	// generateAndAttachBTree(startNode, token);

	printLogicalNodes(startNode);
}
