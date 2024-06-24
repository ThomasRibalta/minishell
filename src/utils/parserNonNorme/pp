#include "../../header/minishell.h"

// Function to count the number of logical nodes (&& and ||)
int countLogicalNodes(t_token* tokens) {
    int count = 0;
    t_token* current = tokens;
    while (current != NULL) {
        if (current->type == TOKEN_LOGICAL_AND || current->type == TOKEN_LOGICAL_OR) {
            count++;
        }
        current = current->next;
    }
    return count;
}

// Function to create a StartNode and allocate memory for logical nodes if present
t_startnode* createAndSetupStartNode(t_token* tokens) {
    t_startnode* startNode = (t_startnode*)malloc(sizeof(t_startnode));
    if (!startNode) {
        perror("Failed to allocate memory for StartNode");
        exit(EXIT_FAILURE);
    }

    // Initially set the hasLogical flag to false and no children
    startNode->haslogical = false;
    startNode->childcount = 0;
    startNode->children = NULL;

    // Count the number of logical nodes
    int logicalNodeCount = countLogicalNodes(tokens);

    // If logical nodes are present, allocate memory for the array of children
    if (logicalNodeCount > 0) {
        startNode->haslogical = true;
        startNode->childcount = logicalNodeCount;
    } else {
        startNode->childcount = 1;
    }
	startNode->children = (t_logicalnode**)malloc(sizeof(t_logicalnode*) * startNode->childcount);
	if (!startNode->children) {
		perror("Failed to allocate memory for logical node pointers in StartNode");
		free(startNode);
		exit(EXIT_FAILURE);
	}
    return startNode;
}

// Initialize function for an AST node
t_astnode* createASTNode(t_nodetype type, char* value) {
    t_astnode* node = (t_astnode*)malloc(sizeof(t_astnode));
    if (!node) {
        perror("Failed to allocate memory for ASTNode");
        exit(EXIT_FAILURE);
    }
    node->type = type;
    node->value = value; // Make a duplicate of the string to avoid potential issues.
    node->left = NULL;
    node->right = NULL;
    node->inputs = NULL;
    node->outputs = NULL;
	node->is_last_command = false;
    return node;
}

// Function to create a logical node (&&, ||, or HOLDER)
t_logicalnode* createLogicalNode(t_nodetype type) {
    t_logicalnode* node = (t_logicalnode*)malloc(sizeof(t_logicalnode));
    if (!node) {
        perror("Failed to allocate memory for LogicalNode");
        exit(EXIT_FAILURE);
    }
    node->type = type;
    node->left = NULL;
    node->right = NULL;
    return node;
}


// Function to add logical nodes to the StartNode's children array, or create a HOLDER node
void addLogicalNodeToStartNode(t_startnode* startNode, t_token* tokens) {
    if (startNode->haslogical) {
        // Allocate logical nodes based on the tokens
        int index = 0;
        t_token* currentToken = tokens;
        while (currentToken != NULL && index < startNode->childcount) {
            if (currentToken->type == TOKEN_LOGICAL_AND || currentToken->type == TOKEN_LOGICAL_OR) {
                t_nodetype type = (currentToken->type == TOKEN_LOGICAL_AND) ? NODE_LOGICAL_AND : NODE_LOGICAL_OR;
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

t_redirection	*createredirection(char *filename, int caracteristic)
{
	t_redirection	*redir;

	redir = (t_redirection *)malloc(sizeof(t_redirection));
	if (!redir)
	{
		perror("Failed to allocate memory for Redirection");
		exit(EXIT_FAILURE);
	}
	redir->filename = filename;
	redir->caracteristic = caracteristic;
	redir->next = NULL;
	return (redir);
}

void addRedirection(t_redirection** list, t_redirection* redir) {
    if (*list == NULL) {
        *list = redir;
    } else {
        t_redirection* current = *list;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = redir;
    }
}

t_astnode* buildCommandPipeTree(t_token** currentToken) {
    t_astnode* root = NULL, *currentCommand = NULL;
    t_redirection* tempInputs = NULL, *tempOutputs = NULL, *tempAppends = NULL, *tempHereDoc = NULL;

    while (*currentToken) {
        switch ((*currentToken)->type) {
            case TOKEN_COMMAND:
            case TOKEN_PAREN:
                // Create command or parenthetical node
                currentCommand = createASTNode((*currentToken)->type == TOKEN_COMMAND ? NODE_COMMAND : NODE_PARENTHESE, (*currentToken)->value);
                if (root == NULL) {
                    root = currentCommand;
                } else {
                    t_astnode* last = root;
                    while (last->right) last = last->right;
                    last->right = currentCommand;
                }
                break;

            case TOKEN_PIPE:
            case TOKEN_LOGICAL_AND:
            case TOKEN_LOGICAL_OR:
                // Handle empty commands between pipes or before logical operators
                if (!currentCommand && (tempInputs || tempOutputs || tempAppends || tempHereDoc)) {
                    currentCommand = createASTNode(NODE_EMPTY_COMMAND, NULL);
                    currentCommand->inputs = tempInputs;
                    currentCommand->outputs = tempOutputs;
                    if (root == NULL) {
                        root = currentCommand;
                    } else {
                        t_astnode* last = root;
                        while (last->right) last = last->right;
                        last->right = currentCommand;
                    }
                }
				else {
					currentCommand->inputs = tempInputs;
                    currentCommand->outputs = tempOutputs;
				}
                // Reset current command after handling pipes
                if ((*currentToken)->type == TOKEN_PIPE) {
                    t_astnode* pipeNode = createASTNode(NODE_PIPE, "|");
                    pipeNode->left = root;
                    root = pipeNode;
                    currentCommand = NULL;
                } else {
                    return root; // Break the loop and return for logical operators
                }
                // Reset redirections after they've been assigned
                tempInputs = tempOutputs = tempAppends = tempHereDoc = NULL;
                break;

            case TOKEN_IN:
    		case TOKEN_OUT:
        	case TOKEN_APPEND:
        	case TOKEN_HEREDOC:
            {
                t_redirection* newRedir = createredirection((*currentToken)->value, (*currentToken)->type == TOKEN_APPEND
				|| (*currentToken)->type == TOKEN_HEREDOC);
                if ((*currentToken)->type == TOKEN_IN || (*currentToken)->type == TOKEN_HEREDOC)
                    addRedirection(&tempInputs, newRedir);
                else if ((*currentToken)->type == TOKEN_OUT || (*currentToken)->type == TOKEN_APPEND)
                    addRedirection(&tempOutputs, newRedir);
            }
            break;
        }
        *currentToken = (*currentToken)->next;  // Move to the next token
    }

    // Final command node setup
    if (currentCommand == NULL && (tempInputs || tempOutputs || tempAppends || tempHereDoc)) {
        currentCommand = createASTNode(NODE_EMPTY_COMMAND, NULL);
        currentCommand->inputs = tempInputs;
        currentCommand->outputs = tempOutputs;
        if (root == NULL) {
            root = currentCommand;
        } else {
            t_astnode* last = root;
            while (last->right) last = last->right;
            last->right = currentCommand;
        }
    }
	else {
		currentCommand->inputs = tempInputs;
        currentCommand->outputs = tempOutputs;
	}

    return root;
}

void generateAndAttachBTree(t_startnode* startNode, t_token* tokens) {
    if (!startNode->haslogical) {
        // If there are no logical operators, build the entire tree and attach to the logical HOLDER node.
        t_logicalnode* holder = startNode->children[0];
        holder->left = buildCommandPipeTree(&tokens); // Since no logical ops, the tree will be directly under the HOLDER.
    } else {
        // If logical operators are present, build separate trees and attach to the correct side of logical nodes.
        int count = -1;

        while (tokens != NULL) {
            // Check for logical operator to increment count and decide where to attach the tree.
            if (tokens->type == TOKEN_LOGICAL_AND || tokens->type == TOKEN_LOGICAL_OR) {
                count++;
                tokens = tokens->next; // Skip the logical operator token.
            } else {
                if (count == -1) {
                    // Attach the first command sequence to the left of the first logical node.
                    startNode->children[0]->left = buildCommandPipeTree(&tokens);
                } else if (count == 0) {
                    // Attach the second command sequence to the right of the first logical node.
                    startNode->children[0]->right = buildCommandPipeTree(&tokens);
                } else {
                    // For all subsequent logical nodes, attach the command sequence to the left.
                    startNode->children[count]->left = buildCommandPipeTree(&tokens);
                }
            }
        }
    }
}

void	parser(t_token *tokens, t_mainstruct mainstruct)
{
	t_startnode	*startnode;

	startnode = createAndSetupStartNode(tokens);
	addLogicalNodeToStartNode(startnode, tokens);
	generateAndAttachBTree(startnode, tokens);
	expenser(startnode);
	executer(startnode, mainstruct);
}
