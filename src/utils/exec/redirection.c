/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 16:03:40 by toto              #+#    #+#             */
/*   Updated: 2024/06/11 21:32:24 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

char	*get_file_name(t_astnode *node)
{
	t_redirection	*tmp;
	
	printf("inputs : %s\n", node->inputs->filename);
	tmp = node->inputs;
	while (tmp->next)
	{
		tmp = tmp->next;
	}
	return (tmp->filename);
}

int	redirection_in(t_astnode *node, int *exit_status, t_command *cmd)
{
	char	*filename;
	int		fd;

	if (node->inputs == NULL)
		return (0);
	filename = get_file_name(node);
	if (node->inputs->caracteristic == 0)
	{
		fd = open(clean_quote(filename), O_RDONLY);
		if (fd == -1)
		{
			perror("open");
			*exit_status = 1;
			return (2);
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	else
	{
		make_pipe(cmd, node);
		return (1);
	}
	return (0);
}

void	handle_child_process(t_astnode *node, t_command *cmd)
{
	if (cmd->here_doc)
	{
		close(cmd->p_id2[1]);
		close(cmd->p_id[0]);
		dup2(cmd->p_id2[0], STDIN_FILENO);
		close(cmd->p_id2[0]);
	}
	if (!(node->is_last_command) || node->outputs != NULL)
	{
		dup2(cmd->fd, STDOUT_FILENO);
		if (!cmd->here_doc)
			close(cmd->p_id[0]);
		close(cmd->fd);
	}
	else
	{
		dup2(cmd->std_out, STDOUT_FILENO);
		close(cmd->std_out);
		close(cmd->fd);
		close(cmd->p_id[0]);
	}
}

void	handle_parent_process(t_astnode *node, t_command *cmd)
{
	if (cmd->here_doc)
	{
		close(cmd->p_id2[0]);
		close(cmd->p_id2[1]);
		close(cmd->p_id[1]);
		if (!(node->is_last_command))
			dup2(cmd->p_id[0], STDIN_FILENO);
		close(cmd->p_id[0]);
	}
	else if (!(node->is_last_command))
	{
		dup2(cmd->p_id[0], STDIN_FILENO);
		close(cmd->p_id[0]);
		close(cmd->p_id[1]);
		//close(cmd->p_id2[0]);
		//cclose(cmd->p_id2[1]);
	}
	else
	{
		dup2(cmd->std_in, STDIN_FILENO);
		close(cmd->std_in);
		close(cmd->p_id[0]);
                close(cmd->p_id[1]);
	}
}

int	open_output_append(t_astnode *node)
{
	int	fd;

	if (node->outputs->caracteristic == 1)
	{
		fd = open(node->outputs->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd == -1)
		{
			perror("open");
			return (-1);
		}
	}
	else
	{
		fd = open(node->outputs->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
		{
			perror("open");
			return (-1);
		}
	}
	return (fd);
}
