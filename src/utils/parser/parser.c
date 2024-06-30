/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedurand <jedurand@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 16:35:26 by jedurand          #+#    #+#             */
/*   Updated: 2024/06/29 23:44:21 by jedurand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

t_astnode	*build_command_pipe_tree(t_token **current_token)
{
	t_astnode		*root;
	t_astnode		*current_command;
	t_redirections	redirs;

	root = NULL;
	current_command = NULL;
	redirs.inputs = NULL;
	redirs.outputs = NULL;
	while (*current_token != NULL)
	{
		if (process_token(current_token, &root, &current_command, &redirs))
			break ;
		*current_token = (*current_token)->next;
	}
	finalize_command(&root, &current_command, &redirs);
	return (root);
}

void	handle_outputs(t_redirection **temp_outputs, t_astnode *current_command)
{
	if (current_command)
		current_command->outputs = *temp_outputs;
	*temp_outputs = NULL;
}

void	generate_and_attach_btree_helper(t_startnode *start_node,
		t_token **current_token, int *count)
{
	if ((*current_token)->type == TOKEN_LOGICAL_AND
		|| (*current_token)->type == TOKEN_LOGICAL_OR)
	{
		(*count)++;
		*current_token = (*current_token)->next;
	}
	else
	{
		if (*count == -1)
			start_node->children[0]->left = build_command_pipe_tree(
					current_token);
		else if (*count == 0)
			start_node->children[0]->right = build_command_pipe_tree(
					current_token);
		else
			start_node->children[*count]->left = build_command_pipe_tree(
					current_token);
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
			generate_and_attach_btree_helper(start_node, &current_token,
				&count);
		}
	}
}

const char* get_node_type_string(t_nodetype type)
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

void print_redirections(const t_redirection *redir)
{
const char*sep;

sep = "";
while (redir)
{
printf("%s%s", sep, redir->filename);
sep = ", ";
redir = redir->next;
}
}

void print_ast(const t_astnode *node, int level)
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

void print_entire_ast(const t_startnode *start_node)
{
int i;

if (!start_node || !start_node->children)
{
printf("No AST data available to display.\n");
return ;
}
printf("Complete Abstract Syntax Tree:\n");
i = 0;
while (i < start_node->childcount)
{
t_logicalnode*logical_node = start_node->children[i];
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

void	parser(t_token *tokens, t_mainstruct mainstruct)
{
	t_startnode	*start_node;

	start_node = create_and_setup_start_node(tokens);
	add_logical_node_to_start_node(start_node, tokens);
	generate_and_attach_btree(start_node, tokens);
	expenser(start_node);
	print_entire_ast(start_node);
	executer(start_node, mainstruct);
}
