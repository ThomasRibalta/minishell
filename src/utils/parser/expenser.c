#include "../../header/minishell.h"

void is_last_command_btree(ASTNode* node)
{
    if (node == NULL)
        return;
	while (node->right != NULL)
	{
		node = node->right;
	}
    node->is_last_command = true;
}

void is_last_command(StartNode* startNode)
{
    int i;

    i = 0;
    if (!startNode->hasLogical)
    {
        is_last_command_btree(startNode->children[0]->left);
    }
    else
    {
        while (i < startNode->childCount)
        {
            is_last_command_btree(startNode->children[i]->left);
            if (i == 0 && startNode->children[i]->right)
            {
                is_last_command_btree(startNode->children[i]->right);
            }
            i++;
        }
    }
}

void expenser(StartNode* startNode)
{
	is_last_command(startNode);
}
