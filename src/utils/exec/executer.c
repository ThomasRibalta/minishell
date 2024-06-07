#include "../../header/minishell.h"

void processBinaryTree2(ASTNode* node, command *cmd, int *exit_status)
{
    if (node == NULL) return;
    processBinaryTree2(node->left, cmd, exit_status);
    if (node->type == NODE_COMMAND)
    {
        if (node->outputs || node->appends)
            execute_output_append_command(node, cmd, exit_status);
        else
        {
            cmd->fd = -1;
            execute_command(node, cmd, exit_status);
        }
    }
    if (node->type == NODE_PARENTHESE)
    {
        execute_output_append_parenthese(node, cmd, exit_status);
    }
    processBinaryTree2(node->right, cmd, exit_status);
}

void execute_logical_and_or(ASTNode* node, char ***env, char ***export, int *exit_status)
{
    command *cmd;
    
    cmd = init_command(dup(STDOUT_FILENO), dup(STDIN_FILENO), env, export);
    processBinaryTree2(node, cmd, exit_status);
    free_command(cmd, exit_status);
}

void expandCommandTrees2(StartNode* startNode, char ***env, char ***export, int *exit_status)
{
    int i;

    i = -1;
    if (!startNode->hasLogical) 
        execute_logical_and_or(startNode->children[0]->left, env, export, exit_status);
    else 
    {
        while (++i < startNode->childCount) 
        {
            if (startNode->children[i]->type == NODE_LOGICAL_AND) 
            {
                if (startNode->children[i]->left && (*exit_status == 0 || i == 0)) 
                    execute_logical_and_or(startNode->children[i]->left, env, export, exit_status);
                if (i == 0 && startNode->children[i]->right && *exit_status == 0) 
                    execute_logical_and_or(startNode->children[i]->right, env, export, exit_status);
            }
            else if (startNode->children[i]->type == NODE_LOGICAL_OR) 
            {
                if (startNode->children[i]->left && (*exit_status != 0 || i == 0)) 
                    execute_logical_and_or(startNode->children[i]->left, env, export, exit_status);
                if (i == 0 && startNode->children[i]->right && *exit_status != 0) 
                    execute_logical_and_or(startNode->children[i]->right, env, export, exit_status);
            }
        }
    }
}

void executer(StartNode* startNode, char ***env, char ***export, int *exit_status)
{
    global_sig = 1;
    expandCommandTrees2(startNode, env, export, exit_status);
    global_sig = 0;
}
