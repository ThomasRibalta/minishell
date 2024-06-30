/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedurand <jedurand@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 02:30:00 by jedurand          #+#    #+#             */
/*   Updated: 2024/06/29 23:46:30 by jedurand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

void	process_command_token(t_astnode **root, t_astnode **cmd, t_token *token)
{
	t_nodetype	node_type;

	node_type = NODE_COMMAND;
	if (token->type == TOKEN_PAREN)
		node_type = NODE_PARENTHESE;
	*cmd = create_ast_node(node_type, token->value);
	if (*root == NULL)
		*root = *cmd;
	else
		add_command_to_tree(root, *cmd);
}

void	handle_pipe_token(t_astnode **root, t_astnode **cmd,
		t_redirections *redirs)
{
	t_astnode	*pipe_node;

	pipe_node = NULL;
	if (!*cmd && (redirs->inputs || redirs->outputs))
		handle_empty_command(root, cmd, redirs);
	else if (*cmd)
	{
		(*cmd)->inputs = redirs->inputs;
		(*cmd)->outputs = redirs->outputs;
	}
	pipe_node = create_ast_node(NODE_PIPE, "|");
	pipe_node->left = *root;
	*root = pipe_node;
	*cmd = NULL;
	redirs->inputs = NULL;
	redirs->outputs = NULL;
}

void	handle_empty_command(t_astnode **root, t_astnode **cmd,
		t_redirections *redirs)
{
	*cmd = create_ast_node(NODE_EMPTY_COMMAND, NULL);
	(*cmd)->inputs = redirs->inputs;
	(*cmd)->outputs = redirs->outputs;
	if (*root == NULL)
		*root = *cmd;
	else
		add_command_to_tree(root, *cmd);
}

void	process_redirection_token(t_token *token, t_redirections *redirs)
{
	t_redirection	*new_redir;
	int				is_append_or_heredoc;

	is_append_or_heredoc = 0;
	if (token->type == TOKEN_APPEND || token->type == TOKEN_HEREDOC)
		is_append_or_heredoc = 1;
	new_redir = create_redirection(token->value, is_append_or_heredoc);
	if (token->type == TOKEN_IN || token->type == TOKEN_HEREDOC)
		add_redirection(&redirs->inputs, new_redir);
	else if (token->type == TOKEN_OUT || token->type == TOKEN_APPEND)
		add_redirection(&redirs->outputs, new_redir);
}

void	add_command_to_tree(t_astnode **root, t_astnode *cmd)
{
	t_astnode	*last;

	last = *root;
	while (last->right)
		last = last->right;
	last->right = cmd;
}
