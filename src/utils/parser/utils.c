#include "../../header/minishell.h"

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

    // Print the node type and value with indentation
    printf("%*s%s", level * 4, "", getNodeTypeString(node->type));
    if (node->value) {
        printf(": %s", node->value);
    }

    // Print redirections if they exist
    if (node->inputs || node->outputs || node->appends || node->here_doc) {
        printf(" [");
        bool first = true;
        if (node->inputs) {
            printf("In: ");
            printRedirections(node->inputs);
            first = false;
        }
        if (node->outputs) {
            printf("%sOut: ", first ? "" : "; ");
            printRedirections(node->outputs);
            first = false;
        }
        if (node->appends) {
            printf("%sAppend: ", first ? "" : "; ");
            printRedirections(node->appends);
            first = false;
        }
        if (node->here_doc) {
            printf("%sHereDoc: ", first ? "" : "; ");
            printRedirections(node->here_doc);
        }
        printf("]");
    }
    printf("\n");

    // Recursively print the left and right subtrees
    printAST(node->left, level + 1);
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
