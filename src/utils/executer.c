#include "../header/minishell.h"

int ft_strcmp(char *value1, char *value2)
{
    while (*value1 && (*value1 == *value2))
    {
        value1++;
        value2++;
    }
    return (*(unsigned char *)value1 - *(unsigned char *)value2);
}

int is_builtin(char *value)
{
    if (!ft_strcmp(value, "cd") ||
        !ft_strcmp(value, "/bin/cd") ||
        !ft_strcmp(value, "unset") ||
        !ft_strcmp(value, "export") ||
        !ft_strcmp(value, "exit"))
    {
        return 1;
    }
    return 0;
}

int is_fork_builtin(char *value)
{
    if (!ft_strcmp(value, "echo") ||
        !ft_strcmp(value, "env") ||
        !ft_strcmp(value, "/bin/env") ||
        !ft_strcmp(value, "/bin/echo") ||
        !ft_strcmp(value, "export") ||
        !ft_strcmp(value, "cd") ||
        !ft_strcmp(value, "pwd") ||
        !ft_strcmp(value, "/bin/cd") ||
        !ft_strcmp(value, "unset"))
    {
        return 1;
    }
    return 0;
}

char *get_path(char **env)
{
    char *path = get_env_value("PATH", env, 0);
    return (path);
}

char *check_path(char **paths, char *name)
{
    char *join;
    char *tmp;
    int i;

    i = 0;
    if (name[0] == '.' && name[1] == '/')
        return (name);
    if (ft_strncmp(name, "/bin/", 5) == 0)
    {
        if (access(name, F_OK | X_OK) == 0)
            return (name);
    }
    while (paths[i])
    {
        tmp = ft_strjoin(paths[i], "/");
        join = ft_strjoin(tmp, name);
        free(tmp);
        if (access(join, F_OK | X_OK) == 0)
            return (join);
        free(join);
        i++;
    }
    return (NULL);
}

void free_all(char **tab)
{
    char **tmp = tab;
    while (*tab) {
        free(*tab);
        tab++;
    }
    free(tmp);
}

void execute(char **param, char *path, char **env)
{
    char **paths;
    char *path1;

    if (param[0][0] == '/' && ft_strcmp(param[0], "/bin/") == 0)
    {
        ft_putendl_fd("minishell: is a directory", 2);
        exit(126);
    }
    if (!path)
    {
        ft_putendl_fd("minishell: No such file or directory", 2);
        exit(127);
    }
    paths = ft_split(path, ':');
    path1 = check_path(paths, clean_quote(param[0]));
    if (!path1 || !paths) {
        ft_putendl_fd("minishell: command not found", 2);
        free_all(paths);
        exit(127);
    }
    if (execve(path1, param, env) == -1) {
        free_all(paths);
    }
    free(path1);
    exit(EXIT_FAILURE);
}

int execute_builtin(char ***env, char ***export, char **param, ASTNode *node, int *exit_status)
{
    if (ft_strcmp(clean_quote(param[0]), "exit") == 0 && node->is_last_command)
        exit_program(param + 1);
    else if ((ft_strcmp(clean_quote(param[0]), "cd") == 0 || ft_strcmp(clean_quote(param[0]), "cd") == 0) && node->is_last_command)
        *exit_status = cd(param + 1, env);
    else if (ft_strcmp(clean_quote(param[0]), "export") == 0 && param[1] != NULL)
        *exit_status = export_var(env, export, param + 1);
    else if (ft_strcmp(clean_quote(param[0]), "unset") == 0)
        *exit_status = unset_var(env, param + 1);
    else 
        return 1;
    return 0;
}

int execute_fork_builtin(char **env, char **export, char **param)
{
    if (ft_strcmp(clean_quote(param[0]), "echo") == 0 || ft_strcmp(clean_quote(param[0]), "/bin/echo") == 0)
        echo(param);
    else if (ft_strcmp(clean_quote(param[0]), "env") == 0 || ft_strcmp(clean_quote(param[0]), "/bin/env") == 0)
        print_env(env, 0);
    else if (ft_strcmp(clean_quote(param[0]), "export") == 0 && param[1] == NULL)
        print_env(export, 1);
    else if (ft_strcmp(clean_quote(param[0]), "cd") == 0 || ft_strcmp(clean_quote(param[0]), "cd") == 0)
        exit(0);
    else if (ft_strcmp(clean_quote(param[0]), "unset") == 0)
        exit(0);
    else if (ft_strcmp(clean_quote(param[0]), "pwd") == 0 || ft_strcmp(clean_quote(param[0]), "/bin/pwd") == 0)
        printf("%s\n", pwd(0));
    else 
        return 1;
    exit(0);
}

void handle_child_process(ASTNode *node, command *cmd, int p_id[2], int fd, int here_doc, int p_id2[2])
{
    if (here_doc)
    {
        close(p_id2[1]);
        close(p_id[0]);
        dup2(p_id2[0], STDIN_FILENO);
        close(p_id2[0]);
    }
    if (!(node->is_last_command) || node->outputs != NULL || node->appends != NULL)
    {
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }
    else
    {
        dup2(cmd->std_out, STDOUT_FILENO);
        close(cmd->std_out);
    }
}

void handle_parent_process(ASTNode *node, command *cmd, int p_id[2], int here_doc, int p_id2[2])
{
    if (here_doc)
    {
        close(p_id2[0]);
        close(p_id2[1]);
        close(p_id[1]);
        if (!(node->is_last_command))
            dup2(p_id[0], STDIN_FILENO);
        close(p_id[0]);
    }
    else if (!(node->is_last_command))
    {
        dup2(p_id[0], STDIN_FILENO);
        close(p_id[0]);
        close(p_id[1]);
    } 
    else 
    {
        dup2(cmd->std_in, STDIN_FILENO);
        close(cmd->std_in);
    }
}

int redirection_in(ASTNode *node, int p_id[2])
{
    char *filename;

    (void)p_id;
    while (node->inputs->next)
        node->inputs = node->inputs->next;
    filename = node->inputs->filename;
    if (node->inputs->caracteristic == 0)
    {
        int fd = open(clean_quote(filename), O_RDONLY);
        if (fd == -1)
        {
            perror("open");
            return 2;
        }
        dup2(fd, STDIN_FILENO);
        close(fd);
    }
    else
        return 1;
    return 0;
}

void execute_command(ASTNode* node, char ***env, char ***export, command *cmd, int fd, int *exit_status)
{
    char **split_nodeValue;
    int p_id[2], p_id2[2];
    pid_t pid;
    int here_doc = 0;

    if (node == NULL || node->value == NULL)
        return;
    if (pipe(p_id) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    if (fd == -1)
        fd = p_id[1];
    if (node->inputs)
        here_doc = redirection_in(node, p_id);
    if (here_doc == 1)
    {
        if (pipe(p_id2) == -1)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
        while (node->inputs->next)
            node->inputs = node->inputs->next;
        write(p_id2[1], node->inputs->filename, ft_strlen(node->inputs->filename));
    }
    if (here_doc == 2)
    {
        *exit_status = 1;
        return ;
    }
    replaceEnvVars(&node->value, *env, exit_status);
    split_nodeValue = ft_split(node->value, ' ');
    split_nodeValue = check_wildcard(split_nodeValue);
    split_nodeValue = clean_quote_all(split_nodeValue);
    if (split_nodeValue[0] == NULL)
        return ((void) ft_putendl_fd("minishell: : command not found", 2));
    if (is_builtin(clean_quote(split_nodeValue[0])))
    {
        execute_builtin(env, export, split_nodeValue, node, exit_status);
        if (*exit_status)
            return ;
    }
    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (pid == 0)
    {
        handle_child_process(node, cmd, p_id, fd, here_doc, p_id2);
        if (is_fork_builtin(clean_quote(split_nodeValue[0])))
            execute_fork_builtin(*env, *export, split_nodeValue);
        execute(split_nodeValue, get_path(*env), *env);
    }
    else
    {
        handle_parent_process(node, cmd, p_id, here_doc, p_id2);
        cmd->pids[cmd->pid_count] = pid;
        (cmd->pid_count)++;
    }
}

char *remove_parenthese(char *str)
{
    char *new_str;

    if (str[ft_strlen(str) - 1] == ')')
    {
        new_str = ft_substr(str, 1, ft_strlen(str) - 2);   
    }else
    {
        new_str = ft_substr(str, 1, ft_strlen(str) - 1);
    }
    return new_str;
}

void execute_parenthese(ASTNode* node, char ***env, char ***export, int fd, command *cmd, int *exit_status)
{
    int p_id[2], p_id2[2];
    pid_t pid;
    int here_doc = 0;

    if (pipe(p_id) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    if (fd == -1)
        fd = p_id[1];
    if (node->inputs)
        here_doc = redirection_in(node, p_id);
    if (here_doc == 1)
    {
        if (pipe(p_id2) == -1)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
        while (node->inputs->next)
            node->inputs = node->inputs->next;
        write(p_id2[1], node->inputs->filename, ft_strlen(node->inputs->filename));
    }
    if (here_doc == 2)
    {
        *exit_status = 1;
        return ;
    }
    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (pid == 0)
    {
        handle_child_process(node, cmd, p_id, fd, here_doc, p_id2);
        lexer(remove_parenthese(node->value), env, export, exit_status);
        exit(EXIT_SUCCESS);
    }
    else 
    {
        handle_parent_process(node, cmd, p_id, here_doc, p_id2);
        waitpid(pid, NULL, 0);
    }
}

int open_output_append(ASTNode *node)
{
    int fd;

    while (node->outputs->next)
    {
        node->outputs = node->outputs->next;
    }
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

void execute_output_append_command(ASTNode* node, char ***env, char ***export,command *cmd, int *exit_status)
{
    int fd;

    if (node->outputs) {
        fd = open_output_append(node);
        if (fd == -1)
        {
            *exit_status = 1;
            return ;
        }
        execute_command(node, env, export, cmd, fd, exit_status);
        close(fd);
    }
    if (!node->is_last_command)
        execute_command(node, env, export, cmd, -1, exit_status);
}

void execute_output_append_parenthese(ASTNode* node, char ***env, char ***export, command *cmd, int *exit_status)
{
    int i;
    int fd;

    i = 0;
    if (node->outputs) {
        i++;
        fd = open_output_append(node);
        if (fd == -1)
            return ;
        execute_parenthese(node, env, export, fd, cmd, exit_status);
        close(fd);
    }
    if (!node->is_last_command || i == 0)
        execute_parenthese(node, env, export, -1, cmd, exit_status);
}

void processBinaryTree2(ASTNode* node, char ***env, char ***export, command *cmd, int *exit_status)
{
    if (node == NULL) return;
    processBinaryTree2(node->left, env, export, cmd, exit_status);
    if (node->type == NODE_COMMAND)
    {
        if (node->outputs || node->appends)
            execute_output_append_command(node, env, export, cmd, exit_status);
        else
            execute_command(node, env, export, cmd, -1, exit_status);
    }
    if (node->type == NODE_PARENTHESE)
    {
        execute_output_append_parenthese(node, env, export, cmd, exit_status);
    }
    processBinaryTree2(node->right, env, export, cmd, exit_status);
}

command *init_command(int test, int test2)
{
    command *cmd;
    
    cmd = malloc(sizeof(command));
    cmd->std_out = test;
    cmd->std_in = test2;
    cmd->pids = malloc(1024 * sizeof(int));
    cmd->pid_count = 0;
    return (cmd);
}

void    free_command(command *cmd, int *exit_status)
{
    int tmp;

    for (int j = 0; j < cmd->pid_count; j++)
        waitpid(cmd->pids[j], &tmp, 0);
    *exit_status = MY_WEXITSTATUS(tmp);
    free(cmd->pids);
    free(cmd);
}

void expandCommandTrees2(StartNode* startNode, char ***env, char ***export, int *exit_status)
{
    int i;

    i = 0;
    if (!startNode->hasLogical) 
    {
        command *cmd = init_command(dup(STDOUT_FILENO), dup(STDIN_FILENO));
        processBinaryTree2(startNode->children[0]->left, env, export, cmd, exit_status);
        free_command(cmd, exit_status);
    } 
    else 
    {
        while (i < startNode->childCount) 
        {
            if (startNode->children[i]->type == NODE_LOGICAL_AND) 
            {
                if (startNode->children[i]->left && (*exit_status == 0 || i == 0)) 
                {
                    command *cmd = init_command(dup(STDOUT_FILENO), dup(STDIN_FILENO));
                    processBinaryTree2(startNode->children[i]->left, env, export, cmd, exit_status);
                    free_command(cmd, exit_status);
                }
                if (i == 0 && startNode->children[i]->right && *exit_status == 0) 
                {
                    command *cmd = init_command(dup(STDOUT_FILENO), dup(STDIN_FILENO));
                    processBinaryTree2(startNode->children[i]->right, env, export, cmd, exit_status);
                    free_command(cmd, exit_status);
                }
            }
            else if (startNode->children[i]->type == NODE_LOGICAL_OR) 
            {
                if (startNode->children[i]->left && (*exit_status != 0 || i == 0)) 
                {
                    command *cmd = init_command(dup(STDOUT_FILENO), dup(STDIN_FILENO));
                    processBinaryTree2(startNode->children[i]->left, env, export, cmd, exit_status);
                    free_command(cmd, exit_status);
                }
                if (i == 0 && startNode->children[i]->right && *exit_status != 0) 
                {
                    command *cmd = init_command(dup(STDOUT_FILENO), dup(STDIN_FILENO));
                    processBinaryTree2(startNode->children[i]->right, env, export, cmd, exit_status);
                    free_command(cmd, exit_status);
                }
            }
            i++;
        }
    }
}

void executer(StartNode* startNode, char ***env, char ***export, int *exit_status)
{
    global_sig = 1;
    expandCommandTrees2(startNode, env, export, exit_status);
    global_sig = 0;
}
