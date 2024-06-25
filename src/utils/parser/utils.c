#include "../../header/minishell.h"

int	count_logical_nodes(t_token *tokens)
{
	int		count;
	t_token	*current;

	count = 0;
	current = tokens;
	while (current != NULL)
	{
		if (current->type == TOKEN_LOGICAL_AND || current->type == TOKEN_LOGICAL_OR)
			count++;
		current = current->next;
	}
	return (count);
}

void	setup_start_node_children(t_startnode *start_node)
{
	start_node->children = (t_logicalnode **)malloc(sizeof(t_logicalnode *) * start_node->childcount);
	if (!start_node->children)
	{
		perror("Failed to allocate memory for logical node in StartNode");
		free(start_node);
		exit(EXIT_FAILURE);
	}
}

void	setup_start_node(t_startnode *start_node, int logical_node_count)
{
	if (logical_node_count > 0)
	{
		start_node->haslogical = true;
		start_node->childcount = logical_node_count;
	}
	else
		start_node->childcount = 1;
	setup_start_node_children(start_node);
}

int	is_logical_operator(t_token *token)
{
	return (token->type == TOKEN_LOGICAL_AND || token->type == TOKEN_LOGICAL_OR);
}

t_astnode	*build_command_pipe_tree_part2(t_token **current_token,
	t_astnode **root, t_astnode **current_command)
{
	t_redirection	*tempInputs;
	t_redirection	*tempOutputs;

	tempInputs = NULL;
	tempOutputs = NULL;
	while (*current_token != NULL)
	{
		if (is_logical_operator(*current_token))
		{
			process_other_tokens(root, current_command);
			update_command_redirections(current_command, tempInputs, tempOutputs);
			return (*root);
		}
		else if (is_redirection(*current_token))
		{
			process_redirection_token(*current_token, &tempInputs, &tempOutputs);
		}
		*current_token = (*current_token)->next;
	}
	process_other_tokens(root, current_command);
	update_command_redirections(current_command, tempInputs, tempOutputs);
	return (*root);
}
