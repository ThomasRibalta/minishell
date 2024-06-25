#include "../../header/minishell.h"

void	add_logical_node(t_startnode *start_node, t_token *current_token, int *index)
{
	t_nodetype	type;

	if (current_token->type == TOKEN_LOGICAL_AND)
		type = NODE_LOGICAL_AND;
	else if (current_token->type == TOKEN_LOGICAL_OR)
		type = NODE_LOGICAL_OR;
	else
		return;
	start_node->children[*index] = create_logical_node(type);
	(*index)++;
}

void	add_logical_node_to_start_node(t_startnode *start_node, t_token *tokens)
{
	int		index;
	t_token	*current_token;

	index = 0;
	current_token = tokens;
	if (start_node->haslogical)
	{
		while (current_token != NULL && index < start_node->childcount)
		{
			add_logical_node(start_node, current_token, &index);
			current_token = current_token->next;
		}
	}
	else
		start_node->children[0] = create_logical_node(NODE_LOGICAL_HOLDER);
}

void	process_command_token(t_astnode **root, t_astnode **current_command, t_token *current_token)
{
	t_nodetype	node_type;
	t_astnode	*last;

	if (current_token->type == TOKEN_COMMAND)
		node_type = NODE_COMMAND;
	else
		node_type = NODE_PARENTHESE;
	*current_command = create_ast_node(node_type, current_token->value);
	if (*root == NULL)
	{
		*root = *current_command;
	}
	else
	{
		last = *root;
		while (last->right != NULL)
			last = last->right;
		last->right = *current_command;
	}
}

void	handle_pipe_token(t_astnode **root, t_astnode **current_command)
{
	t_astnode	*pipe_node;

	pipe_node = create_ast_node(NODE_PIPE, "|");
	pipe_node->left = *root;
	*root = pipe_node;
	*current_command = NULL;
}

void	process_other_tokens(t_astnode **root, t_astnode **current_command)
{
	t_astnode	*last;

	if (!*current_command)
	{
		*current_command = create_ast_node(NODE_EMPTY_COMMAND, NULL);
		if (*root == NULL)
		{
			*root = *current_command;
		}
		else
		{
			last = *root;
			while (last->right != NULL)
				last = last->right;
			last->right = *current_command;
			(*current_command)->inputs = NULL;
			(*current_command)->outputs = NULL;
		}
	}
}
