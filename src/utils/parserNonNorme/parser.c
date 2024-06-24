#include "../../header/minishell.h"

const char	*get_node_type_string(t_nodetype type)
{
	if (type == NODE_COMMAND)
		return ("COMMAND");
	if (type == NODE_EMPTY_COMMAND)
		return ("EMPTY COMMAND");
	if (type == NODE_PARENTHESE)
		return ("PARENTHESE");
	if (type == NODE_LOGICAL_AND)
		return ("LOGICAL AND");
	if (type == NODE_LOGICAL_OR)
		return ("LOGICAL OR");
	if (type == NODE_PIPE)
		return ("PIPE");
	if (type == NODE_LOGICAL_HOLDER)
		return ("LOGICAL HOLDER");
	return ("UNKNOWN");
}

void	print_redirections(const t_redirection *redir)
{
	const char	*sep;

	sep = "";
	while (redir)
	{
		printf("%s%s", sep, redir->filename);
		sep = ", ";
		redir = redir->next;
	}
}

void	print_ast(const t_astnode *node, int level)
{
	if (node == NULL)
		return ;
	printf("%*s%s", level * 4, "", get_node_type_string(node->type));
	if (node->value)
		printf(": %s", node->value);
	if (node->inputs || node->outputs)
	{
		printf(" [");
		if (node->inputs)
		{
			printf("In: ");
			print_redirections(node->inputs);
		}
		if (node->outputs)
		{
			printf("; Out: ");
			print_redirections(node->outputs);
		}
		printf("]");
	}
	printf("\n");
	print_ast(node->left, level + 1);
	print_ast(node->right, level + 1);
}

void	print_entire_ast(const t_startnode *start_node)
{
	int	i;

	if (!start_node || !start_node->children)
	{
		printf("No AST data available to display.\n");
		return ;
	}
	printf("Complete Abstract Syntax Tree:\n");
	i = 0;
	while (i < start_node->childcount)
	{
		t_logicalnode *logical_node = start_node->children[i];
		if (i > 0)
		{
			printf("Subtree %d:\n", i + 1);
			printf("Logical Node (%s):\n",
				get_node_type_string(logical_node->type));
		}
		if (logical_node->left)
		{
			printf("Left:\n");
			print_ast(logical_node->left, 1);
		}
		if (i == 0 && logical_node->type != NODE_LOGICAL_HOLDER)
		{
			printf("Logical Node (%s):\n",
				get_node_type_string(logical_node->type));
		}
		if (logical_node->right)
		{
			printf("Right:\n");
			print_ast(logical_node->right, 1);
		}
		if (i < start_node->childcount - 1)
			printf("\n");
		i++;
	}
}

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

t_startnode	*create_start_node(void)
{
	t_startnode	*start_node;

	start_node = (t_startnode *)malloc(sizeof(t_startnode));
	if (!start_node)
	{
		perror("Failed to allocate memory for StartNode");
		exit(EXIT_FAILURE);
	}
	start_node->haslogical = false;
	start_node->childcount = 0;
	start_node->children = NULL;
	return (start_node);
}

void	setup_start_node_children(t_startnode *start_node)
{
	start_node->children = (t_logicalnode **)malloc(sizeof(t_logicalnode *) * start_node->childcount);
	if (!start_node->children)
	{
		perror("Failed to allocate memory for logical node pointers in StartNode");
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

t_startnode	*create_and_setup_start_node(t_token *tokens)
{
	t_startnode	*start_node;
	int			logical_node_count;

	start_node = create_start_node();
	logical_node_count = count_logical_nodes(tokens);
	setup_start_node(start_node, logical_node_count);
	return (start_node);
}

t_astnode	*create_ast_node(t_nodetype type, char *value)
{
	t_astnode	*node;

	node = (t_astnode *)malloc(sizeof(t_astnode));
	if (!node)
	{
		perror("Failed to allocate memory for ASTNode");
		exit(EXIT_FAILURE);
	}
	node->type = type;
	node->value = value;
	node->left = NULL;
	node->right = NULL;
	node->inputs = NULL;
	node->outputs = NULL;
	node->is_last_command = false;
	return (node);
}

t_logicalnode	*create_logical_node(t_nodetype type)
{
	t_logicalnode	*node;

	node = (t_logicalnode *)malloc(sizeof(t_logicalnode));
	if (!node)
	{
		perror("Failed to allocate memory for LogicalNode");
		exit(EXIT_FAILURE);
	}
	node->type = type;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

void	add_logical_node(t_startnode *start_node, t_token *current_token, int *index)
{
	t_nodetype type;

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

t_redirection	*create_redirection(char *filename, int characteristic)
{
	t_redirection	*redir;

	redir = (t_redirection *)malloc(sizeof(t_redirection));
	if (!redir)
	{
		perror("Failed to allocate memory for Redirection");
		exit(EXIT_FAILURE);
	}
	redir->filename = filename;
	redir->caracteristic = characteristic;
	redir->next = NULL;
	return (redir);
}

void	add_redirection(t_redirection **list, t_redirection *redir)
{
	t_redirection	*current;

	if (*list == NULL)
		*list = redir;
	else
	{
		current = *list;
		while (current->next != NULL)
			current = current->next;
		current->next = redir;
	}
}

void	process_command_token(t_astnode **root, t_astnode **current_command, t_token *current_token)
{
	*current_command = create_ast_node(
		current_token->type == TOKEN_COMMAND ? NODE_COMMAND : NODE_PARENTHESE, current_token->value);
	if (*root == NULL)
		*root = *current_command;
	else
	{
		t_astnode	*last = *root;
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

void	process_redirection_token(t_token *current_token, t_redirection **temp_inputs, t_redirection **temp_outputs)
{
	t_redirection	*new_redir;

	new_redir = create_redirection(current_token->value,
		(current_token->type == TOKEN_APPEND || current_token->type == TOKEN_HEREDOC));
	if (current_token->type == TOKEN_IN || current_token->type == TOKEN_HEREDOC)
		add_redirection(temp_inputs, new_redir);
	else if (current_token->type == TOKEN_OUT || current_token->type == TOKEN_APPEND)
		add_redirection(temp_outputs, new_redir);
}

void	process_other_tokens(t_astnode **root, t_astnode **current_command)
{
	if (!*current_command)
	{
		*current_command = create_ast_node(NODE_EMPTY_COMMAND, NULL);
		if (*root == NULL)
			*root = *current_command;
		else
		{
			t_astnode	*last = *root;
			while (last->right != NULL)
				last = last->right;
			last->right = *current_command;
			(*current_command)->inputs = NULL;
			(*current_command)->outputs = NULL;
		}
	}
}

void	update_command_redirections(t_astnode **current_command, t_redirection *temp_inputs, t_redirection *temp_outputs)
{
	if (*current_command)
	{
		(*current_command)->inputs = temp_inputs;
		(*current_command)->outputs = temp_outputs;
	}
}

int	is_logical_operator(t_token *token)
{
	return (token->type == TOKEN_LOGICAL_AND || token->type == TOKEN_LOGICAL_OR);
}

int	is_redirection(t_token *token)
{
	return (token->type == TOKEN_IN || token->type == TOKEN_OUT ||
			token->type == TOKEN_APPEND || token->type == TOKEN_HEREDOC);
}

t_astnode	*build_command_pipe_tree_part2(t_token **current_token, t_astnode **root, t_astnode **current_command)
{
	t_redirection	*temp_inputs = NULL;
	t_redirection	*temp_outputs = NULL;

	while (*current_token != NULL)
	{
		if (is_logical_operator(*current_token))
		{
			process_other_tokens(root, current_command);
			update_command_redirections(current_command, temp_inputs, temp_outputs);
			return (*root);
		}
		else if (is_redirection(*current_token))
		{
			process_redirection_token(*current_token, &temp_inputs, &temp_outputs);
		}
		*current_token = (*current_token)->next;
	}
	process_other_tokens(root, current_command);
	update_command_redirections(current_command, temp_inputs, temp_outputs);
	return (*root);
}

t_astnode	*build_command_pipe_tree_part1(t_token **current_token, t_astnode **root, t_astnode **current_command)
{
	while (*current_token != NULL)
	{
		if ((*current_token)->type == TOKEN_COMMAND || (*current_token)->type == TOKEN_PAREN)
			process_command_token(root, current_command, *current_token);
		else if ((*current_token)->type == TOKEN_PIPE)
			handle_pipe_token(root, current_command);
		else
			break ;
		*current_token = (*current_token)->next;
	}
	return (*root);
}

t_astnode	*build_command_pipe_tree(t_token **current_token)
{
	t_astnode	*root = NULL;
	t_astnode	*current_command = NULL;

	root = build_command_pipe_tree_part1(current_token, &root, &current_command);
	return (build_command_pipe_tree_part2(current_token, &root, &current_command));
}

void	generate_and_attach_btree_helper(t_startnode *start_node, t_token **current_token, int *count)
{
	if ((*current_token)->type == TOKEN_LOGICAL_AND || (*current_token)->type == TOKEN_LOGICAL_OR)
	{
		(*count)++;
		*current_token = (*current_token)->next;
	}
	else
	{
		if (*count == -1)
			start_node->children[0]->left = build_command_pipe_tree(current_token);
		else if (*count == 0)
			start_node->children[0]->right = build_command_pipe_tree(current_token);
		else
			start_node->children[*count]->left = build_command_pipe_tree(current_token);
	}
}

void	generate_and_attach_btree(t_startnode *start_node, t_token *tokens)
{
	t_token	*current_token;
	int		count;

	current_token = tokens;
	count = -1;
	if (!start_node->haslogical)
		start_node->children[0]->left = build_command_pipe_tree(&tokens);
	else
	{
		while (current_token != NULL)
		{
			generate_and_attach_btree_helper(start_node, &current_token, &count);
		}
	}
}

void	parser(t_token *tokens, t_mainstruct mainstruct)
{
	t_startnode	*start_node;

	start_node = create_and_setup_start_node(tokens);
	add_logical_node_to_start_node(start_node, tokens);
	generate_and_attach_btree(start_node, tokens);
	print_entire_ast(start_node);
	expenser(start_node);
	executer(start_node, mainstruct);
}

