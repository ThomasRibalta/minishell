#include "../../header/minishell.h"

int redirection_in(ASTNode *node, int *exit_status, command *cmd)
{
    char *filename;

    while (node->inputs->next)
        node->inputs = node->inputs->next;
    filename = node->inputs->filename;
    if (node->inputs->caracteristic == 0)
    {
        int fd = open(clean_quote(filename), O_RDONLY);
        if (fd == -1)
        {
            perror("open");
            *exit_status = 1;
            return 2;
        }
        dup2(fd, STDIN_FILENO);
        close(fd);
    }
    else
    {
        make_pipe(cmd, node);
        return 1;
    }
    return 0;
}

void handle_child_process(ASTNode *node, command *cmd)
{
    if (cmd->here_doc)
    {
        close(cmd->p_id2[1]);
        close(cmd->p_id[0]);
        dup2(cmd->p_id2[0], STDIN_FILENO);
        close(cmd->p_id2[0]);
    }
    if (!(node->is_last_command) || node->outputs != NULL || node->appends != NULL)
    {
        dup2(cmd->fd, STDOUT_FILENO);
        close(cmd->fd);
    }
    else
    {
        dup2(cmd->std_out, STDOUT_FILENO);
        close(cmd->std_out);
    }
}

void handle_parent_process(ASTNode *node, command *cmd)
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
    } 
    else 
    {
        dup2(cmd->std_in, STDIN_FILENO);
        close(cmd->std_in);
    }
}

int open_output_append(ASTNode *node)
{
    int fd;

    while (node->outputs->next)
        node->outputs = node->outputs->next;
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
    return fd;
}