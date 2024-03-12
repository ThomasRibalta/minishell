#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    NODE_COMMAND,      // 0 - Représente une commande
    NODE_ARGUMENT,     // 1 - Représente un argument de la commande
    NODE_LOGICAL_AND,  // 2 - Représente l'opérateur logique 'ET'
    NODE_PIPE,         // 3 - Représente un pipeline entre deux commandes
    NODE_LOGICAL_OR    // 4 - Représente l'opérateur logique 'OU'
} NodeType;

typedef struct ASTNode {
    NodeType type;
    char* value; // Stocke la commande ou l'argument
    struct ASTNode* left; // Utilisé pour les arbres binaires (non utilisé dans cet exemple)
    struct ASTNode* right; // Chaînage des arguments ou des opérations
} ASTNode;

ASTNode* createASTNode(NodeType type, const char* value) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (!node) {
        perror("Allocation error");
        exit(EXIT_FAILURE);
    }
    node->type = type;
    node->value = value ? strdup(value) : NULL;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void addArgument(ASTNode* commandNode, ASTNode* argNode) {
    ASTNode* current = commandNode;
    while (current->right != NULL) {
        current = current->right;
    }
    current->right = argNode; // Ajoute l'argument à la fin de la chaîne
}

void freeAST(ASTNode* node) {
    if (node == NULL) return;
    freeAST(node->left);
    freeAST(node->right);
    if (node->value) free(node->value);
    free(node);
}

// int main() {
//     // Exemple de création d'un nœud de commande avec des arguments
//     ASTNode* cmd = createASTNode(NODE_COMMAND, "cmd");
//     addArgument(cmd, createASTNode(NODE_ARGUMENT, "arg1"));
//     addArgument(cmd, createASTNode(NODE_ARGUMENT, "arg2"));
//     addArgument(cmd, createASTNode(NODE_ARGUMENT, "arg3"));

//     // Libération de la mémoire
//     freeAST(cmd);

//     return 0;
// }
