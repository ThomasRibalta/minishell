#include "parser.h"

// Function to get a string representation of the node type
const char* getNodeTypeString(NodeType type) {
    switch (type) {
        case NODE_COMMAND: return "COMMAND";
        case NODE_EMPTY_COMMAND: return "EMPTY COMMAND";
        case NODE_PARENTHESE: return "PARENTHESE";
        case NODE_LOGICAL_AND: return "LOGICAL AND";
        case NODE_LOGICAL_OR: return "LOGICAL OR";
        case NODE_PIPE: return "PIPE";
        case NODE_LOGICAL_HOLDER: return "LOGICAL HOLDER";
        default: return "UNKNOWN";
    }
}

// Helper function to print redirections with proper formatting
void printRedirections(const Redirection* redir) {
    const char* sep = "";  // Start with no separator
    while (redir) {
        printf("%s%s", sep, redir->filename);
        sep = ", ";  // Update separator after the first item
        redir = redir->next;
    }
}

void printAST(const ASTNode* node, int level) {
    if (node == NULL) return;

    // Print left subtree first
    printAST(node->left, level + 1);

    // Print current node with indentation
    printf("%*s%s", level * 4, "", getNodeTypeString(node->type));
    if (node->value) {
        printf(": %s", node->value);
    }
    if (node->inputs || node->outputs || node->appends || node->here_doc) {
        printf(" [");
        bool first = true;
        if (node->inputs) {
            printf("In: %s", node->inputs->filename);
            first = false;
        }
        if (node->outputs) {
            printf("%sOut: %s", first ? "" : "; ", node->outputs->filename);
            first = false;
        }
        if (node->appends) {
            printf("%sAppend: %s", first ? "" : "; ", node->appends->filename);
            first = false;
        }
        if (node->here_doc) {
            printf("%sHereDoc: %s", first ? "" : "; ", node->here_doc->filename);
        }
        printf("]");
    }
    printf("\n");

    // Print right subtree last
    printAST(node->right, level + 1);
}

void printEntireAST(const StartNode* startNode) {
    if (!startNode || !startNode->children) {
        printf("No AST data available to display.\n");
        return;
    }

    printf("Complete Abstract Syntax Tree:\n");
    for (int i = 0; i < startNode->childCount; ++i) {
        LogicalNode* logicalNode = startNode->children[i];

        // Print logical node at the beginning for all except the first node
        if (i > 0) {
            printf("Subtree %d:\n", i + 1);
            printf("Logical Node (%s):\n", getNodeTypeString(logicalNode->type));
        }

        if (logicalNode->left) {
            printf("Left:\n");
            printAST(logicalNode->left, 1);
        }

        // Print logical node for the first subtree in the middle
        if (i == 0 && logicalNode->type != NODE_LOGICAL_HOLDER) {
            printf("Logical Node (%s):\n", getNodeTypeString(logicalNode->type));
        }

        if (logicalNode->right) {
            printf("Right:\n");
            printAST(logicalNode->right, 1);
        }

        // Separate the subtrees visually
        if (i < startNode->childCount - 1) {
            printf("\n");
        }
    }
}

void	free_lexer(Token **lexer)
{
	Token *current = *lexer;
	Token *next = (*lexer)->next;

	while (current->next)
	{
		next = current->next;
		//free(current->value);
		free(current);
		current = next;
	}
	*lexer = NULL;
}

// !!!!!!!!!!!!!!!!!!!!!!!!!!! need to free the filename
void freeRedirectionList(Redirection** list) {
    if (list == NULL || *list == NULL) {
        return; // Safety check if list is NULL or already empty
    }

    Redirection* current = *list;
    Redirection* next = NULL;

    while (current != NULL) {
        next = current->next;
        //free(current->filename); // Free the filename string allocated with strdup
        free(current); // Free the redirection node itself
        current = next;
    }

    *list = NULL; // Set the list pointer to NULL after freeing
}
