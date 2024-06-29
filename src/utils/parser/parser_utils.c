/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedurand <jedurand@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 02:30:00 by jedurand          #+#    #+#             */
/*   Updated: 2024/06/29 03:04:09 by jedurand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

int	process_token(t_token **token, t_astnode **root, t_astnode **cmd,
		t_redirection **inputs, t_redirection **outputs)
{
	if ((*token)->type == TOKEN_COMMAND || (*token)->type == TOKEN_PAREN)
		process_command_token(root, cmd, *token);
	else if ((*token)->type == TOKEN_PIPE)
	{
		handle_pipe_token(root, cmd, inputs, outputs);
		*inputs = NULL;
		*outputs = NULL;
	}
	else if (is_redirection(*token))
		process_redirection_token(*token, inputs, outputs);
	else if (is_logical_operator(*token))
		return (1);
	return (0);
}

void	process_command_token(t_astnode **root, t_astnode **cmd, t_token *token)
{
	t_nodetype	node_type;

	node_type = (token->type == TOKEN_COMMAND) ? NODE_COMMAND : NODE_PARENTHESE;
	*cmd = create_ast_node(node_type, token->value);
	if (*root == NULL)
		*root = *cmd;
	else
		add_command_to_tree(root, *cmd);
}

void	handle_pipe_token(t_astnode **root, t_astnode **cmd,
		t_redirection **inputs, t_redirection **outputs)
{
	t_astnode	*pipe_node;

	if (!*cmd && (*inputs || *outputs))
	{
		*cmd = create_ast_node(NODE_EMPTY_COMMAND, NULL);
		(*cmd)->inputs = *inputs;
		(*cmd)->outputs = *outputs;
		if (*root == NULL)
			*root = *cmd;
		else
			add_command_to_tree(root, *cmd);
	}
	else if (*cmd)
	{
		(*cmd)->inputs = *inputs;
		(*cmd)->outputs = *outputs;
	}
	pipe_node = create_ast_node(NODE_PIPE, "|");
	pipe_node->left = *root;
	*root = pipe_node;
	*cmd = NULL;
}

void	process_redirection_token(t_token *token,
		t_redirection **inputs, t_redirection **outputs)
{
	t_redirection	*new_redir;

	new_redir = create_redirection(token->value,
			token->type == TOKEN_APPEND || token->type == TOKEN_HEREDOC);
	if (token->type == TOKEN_IN || token->type == TOKEN_HEREDOC)
		add_redirection(inputs, new_redir);
	else if (token->type == TOKEN_OUT || token->type == TOKEN_APPEND)
		add_redirection(outputs, new_redir);
}

void	finalize_command(t_astnode **root, t_astnode **cmd,
		t_redirection **inputs, t_redirection **outputs)
{
	if (!*cmd && (*inputs || *outputs))
	{
		*cmd = create_ast_node(NODE_EMPTY_COMMAND, NULL);
		if (*root == NULL)
			*root = *cmd;
		else
			add_command_to_tree(root, *cmd);
	}
	if (*cmd)
	{
		(*cmd)->inputs = *inputs;
		(*cmd)->outputs = *outputs;
	}
}

void	add_command_to_tree(t_astnode **root, t_astnode *cmd)
{
	t_astnode	*last;

	last = *root;
	while (last->right)
		last = last->right;
	last->right = cmd;
}

int	is_redirection(t_token *token)
{
	return (token->type == TOKEN_IN || token->type == TOKEN_OUT
		|| token->type == TOKEN_APPEND || token->type == TOKEN_HEREDOC);
}

int	is_logical_operator(t_token *token)
{
	return (token->type == TOKEN_LOGICAL_AND
		|| token->type == TOKEN_LOGICAL_OR);
}
