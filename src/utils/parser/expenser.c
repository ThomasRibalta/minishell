#include "../../header/minishell.h"

void	is_last_command_btree(t_astnode *node)
{
	if (node == NULL)
		return;
	while (node->right != NULL)
	{
		node = node->right;
	}
	node->is_last_command = true;
}

void	is_last_command(t_startnode *startNode)
{
	int i;

	i = 0;
	if (!startNode->haslogical)
	{
		is_last_command_btree(startNode->children[0]->left);
	}
	else
	{
		while (i < startNode->childcount)
		{
			// For the left side of the logical node
			if (startNode->children[i]->left)
			{
				is_last_command_btree(startNode->children[i]->left);
			}

			// For the right side of the logical node if it exists
			if (i == 0 && startNode->children[i]->right)
			{
				is_last_command_btree(startNode->children[i]->right);
			}

			// If this is the last logical node and it doesn't have a right child
			if (i == startNode->childcount - 1 && !startNode->children[i]->right)
			{
				is_last_command_btree(startNode->children[i]->left);
			}
			i++;
		}
	}
}

void	expenser(t_startnode *startNode)
{
	is_last_command(startNode);
}
