/** ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expenser.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 14:47:06 by toto              #+#    #+#             */
/*   Updated: 2024/06/08 14:47:07 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"
/*
void	is_last_command_btree(t_astnode *node)
{
	if (node == NULL)
		return ;
	while (node->right != NULL)
	{
		node = node->right;
	}
	node->is_last_command = true;
}

void	is_last_command(t_startnode *startNode)
{
	int	i;

	i = 0;
	if (!startNode->haslogical)
	{
		is_last_command_btree(startNode->children[0]->left);
	}
	else
	{
		while (i < startNode->childcount)
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

void	expenser(t_startnode *startNode)
{
	is_last_command(startNode);
}*/

void processBinaryTree(t_astnode* node, void (*processStr)(char**)) {
    if (node == NULL) return;

    // Process the left subtree first
    processBinaryTree(node->left, processStr);

    // Process the command value only for COMMAND nodes
    if (node->type == NODE_COMMAND) {
        processStr(&node->value);
    }

    // Process all redirections except for heredoc
    t_redirection* redir = node->inputs;
    while (redir) {
        processStr(&redir->filename);
        redir = redir->next;
    }

    redir = node->outputs;
    while (redir) {
        processStr(&redir->filename);
        redir = redir->next;
    }
    // Process the right subtree last
    processBinaryTree(node->right, processStr);
}

void expandCommandTrees(t_startnode* startNode, void (*processStr)(char**)) {
    if (!startNode->haslogical) {
        // If there are no logical operators, process the entire tree under the HOLDER node.
        processBinaryTree(startNode->children[0]->left, processStr);
    } else {
        // If logical operators are present, process each tree attached to the logical nodes.
        for (int i = 0; i < startNode->childcount; i++) {
            if (startNode->children[i]->left) {
                processBinaryTree(startNode->children[i]->left, processStr);
            }
            if (i == 0 && startNode->children[i]->right) {
                // For the first logical node, also process the right subtree
                processBinaryTree(startNode->children[i]->right, processStr);
            }
        }
    }
}


void printCommandNode(char **str) {
	printf("%s\n", *str);
}

void is_last_command_btree(t_astnode* node) {
    if (node == NULL) return;

	while (node->right != NULL)
	{
		node = node->right;
	}
    node->is_last_command = true;
}

void is_last_command(t_startnode* startNode) {
    if (!startNode->haslogical) {
        is_last_command_btree(startNode->children[0]->left);
    } else {
        for (int i = 0; i < startNode->childcount; i++) {
            is_last_command_btree(startNode->children[i]->left);
            if (i == 0 && startNode->children[i]->right) {
                is_last_command_btree(startNode->children[i]->right);
            }
        }
    }
}

// Si node commande, si arguement, supprimer tout les inputs, sinon garder que le dernier
// Si node parenthese, si arguement premiere commande, supprimer tout les inputs,
// 	sinon pipe le dernier input, sinon aucun input, envoyer le pipe de la commande precedente

void expenser(t_startnode* startNode)
{
	expandCommandTrees(startNode, printCommandNode);
	is_last_command(startNode);
}
