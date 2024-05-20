#include "../header/minishell.h"

#include <string.h>

int ft_strcmp(char *value1, char *value2) {
    while (*value1 && (*value1 == *value2)) {
        value1++;
        value2++;
    }
    return (*(unsigned char *)value1 - *(unsigned char *)value2);
}

// Fonction is_builtin pour vérifier si une commande est intégrée
int is_builtin(char *value) {
    if (!ft_strcmp(value, "cd") ||
        !ft_strcmp(value, "unset") ||
        !ft_strcmp(value, "export") ||
        !ft_strcmp(value, "exit")) {
        return 1;
    }
    return 0;
}

int is_fork_builtin(char *value) {
    if (!ft_strcmp(value, "echo") ||
        !ft_strcmp(value, "env") ||
        !ft_strcmp(value, "export")) {
        return 1;
    }
    return 0;
}

// Fonction get_path pour obtenir le chemin de l'environnement
char *get_path(char **env) {
    while (ft_strncmp("PATH=", *env, 5) != 0 && *env && env)
        env++;
    return (*env + 5);
}

// Fonction check_path pour vérifier si le chemin d'accès est valide
char *check_path(char **paths, char *name) {
    char *join;
    char *tmp;
    int i = 0;
    while (paths[i]) {
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

// Fonction pour libérer la mémoire allouée pour un tableau de chaînes de caractères
void free_all(char **tab) {
    char **tmp = tab;
    while (*tab) {
        free(*tab);
        tab++;
    }
    free(tmp);
}

// Fonction pour exécuter une commande avec les paramètres spécifiés
void execute(char **param, char *path, char **env) {
    char **paths;
    char *path1;
    paths = ft_split(path, ':');
    path1 = check_path(paths, clean_quote(param[0]));
    if (!path1) {
        //free_all(paths);
        //free_all(param);
    }
    if (execve(path1, param, env) == -1) {
        //free_all(paths);
        //free_all(param);
    }
    //free_all(paths);
    //free_all(param);
    //free(path1);
}

int execute_builtin(ASTNode *node, char **env, char **param){
    if (ft_strcmp(clean_quote(param[0]), "exit") == 0)
        exit_program();
    else if (ft_strcmp(clean_quote(param[0]), "cd") == 0)
        cd(param + 1);
    else if (ft_strcmp(clean_quote(param[0]), "export") == 0 && param[1] != NULL)
        export_var(&env, param + 1);
    else if (ft_strcmp(clean_quote(param[0]), "unset") == 0)
        unset_var(&env, param + 1);
    else 
        return 1;
    return 0;
}

int execute_fork_builtin(ASTNode *node, char **env, char **param){
    if (ft_strcmp(clean_quote(param[0]), "echo") == 0)
        echo(param + 1);
    else if (ft_strcmp(clean_quote(param[0]), "env") == 0)
        print_env(env);
    else if (ft_strcmp(clean_quote(param[0]), "export") == 0)
        print_env(env);
    else 
        return 1;
    exit(0);
}

void handle_child_process(ASTNode* node, command *cmd, int p_id[2], char **split_nodeValue, char **env, int fd) {
    if (!(node->is_last_command) || node->outputs != NULL || node->appends != NULL)
    {
        //printf("p_id[2]=%d, fd=%d\n", p_id[1], fd);
        dup2(fd, STDOUT_FILENO);
        close(p_id[0]);
    }
    else
    {
        dup2(cmd->std_out, STDOUT_FILENO);
        close(cmd->std_out);
    }
    if (is_fork_builtin(clean_quote(split_nodeValue[0])))
    {
        execute_fork_builtin(node, env, split_nodeValue);
        exit(EXIT_SUCCESS);
    }
    execute(split_nodeValue, get_path(env), env);
}

void handle_parent_process(ASTNode* node, command *cmd, int p_id[2], pid_t pid) {
    if (!(node->is_last_command))
    {
        dup2(p_id[0], STDIN_FILENO);
        close(p_id[1]);
    } 
    else 
    {
        dup2(cmd->std_in, STDIN_FILENO);
        close(cmd->std_in);
    }
    cmd->pids[cmd->pid_count] = pid;
    (cmd->pid_count)++;
}


void redirection_in(char *filename)
{
    int fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }
    dup2(fd, STDIN_FILENO);
    close(fd);
}

void execute_command(ASTNode* node, char **env, command *cmd, int fd) {
    char **split_nodeValue;
    int p_id[2];
    pid_t pid;

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
        redirection_in(node->inputs->filename);
    split_nodeValue = ft_split(node->value, ' ');
    if (is_builtin(clean_quote(split_nodeValue[0])))
    {
        execute_builtin(node, env, split_nodeValue);
        return ;
    }
    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (pid == 0)
    {
        handle_child_process(node, cmd, p_id, split_nodeValue, env, fd);
    }
    else
        handle_parent_process(node, cmd, p_id, pid);
}

void execute_parenthese(ASTNode* node, char **env) {
    int p_id[2];
    pid_t pid;

    if (pipe(p_id) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (pid == 0) {
        close(p_id[0]);
        dup2(p_id[1], STDOUT_FILENO);
        //open_create_output_file(node);
        lexer(ft_substr(node->value, 1, ft_strlen(node->value) - 2), env);
        exit(EXIT_SUCCESS);
    } else {
        waitpid(pid, NULL, 0);
        close(p_id[1]);
        dup2(p_id[0], STDIN_FILENO);
    }
}

void execute_output_append(ASTNode* node, char **env, command *cmd) {
    if (node->outputs){
        while (node->outputs) {
            int fd = open(node->outputs->filename, O_WRONLY | O_CREAT, 0644);
            if (fd == -1) {
                perror("open");
                exit(EXIT_FAILURE);
            }
            //printf("fd = %d\n", fd);
            execute_command(node, env, cmd, fd);
            close(fd);
            node->outputs = node->outputs->next;
        }
    }
    if (node->appends)
    {
        while (node->appends) {
            int fd = open(node->appends->filename, O_WRONLY | O_APPEND | O_CREAT, 0644);
            if (fd == -1) {
                perror("open");
                exit(EXIT_FAILURE);
            }
            execute_command(node, env, cmd, fd);
            close(fd);
            node->appends = node->appends->next;
        }
    }
    if (!node->is_last_command)
        execute_command(node, env, cmd, -1);
}

void processBinaryTree2(ASTNode* node, char **env, command *cmd) {
    if (node == NULL) return;
    processBinaryTree2(node->left, env, cmd);
    if (node->type == NODE_COMMAND) {
        if (node->outputs || node->appends)
            execute_output_append(node, env, cmd);
        else
        {
           // printf("node->value = %s\n", node->value);
            execute_command(node, env, cmd, -1);
        }
    }
    if (node->type == NODE_PARENTHESE) {
        execute_parenthese(node, env);
    }
    processBinaryTree2(node->right, env, cmd);
}

command *init_command(int test, int test2) {
    command *cmd = malloc(sizeof(command));
    cmd->std_out = test;
    cmd->std_in = test2;
    cmd->pids = malloc(1024 * sizeof(int));
    cmd->pid_count = 0;
    return cmd;
}

void    free_command(command *cmd)
{
    for (int j = 0; j < cmd->pid_count; j++)
        waitpid(cmd->pids[j], NULL, 0);
    free(cmd->pids);
    free(cmd);
}

void expandCommandTrees2(StartNode* startNode, char **env) {
    if (!startNode->hasLogical) 
    {
        command *cmd = init_command(dup(STDOUT_FILENO), dup(STDIN_FILENO));
        processBinaryTree2(startNode->children[0]->left, env, cmd);
        free_command(cmd);
    } 
    else 
    {
        for (int i = 0; i < startNode->childCount; i++) 
        {
            if (startNode->children[i]->left) 
            {
                command *cmd = init_command(dup(STDOUT_FILENO), dup(STDIN_FILENO));
                processBinaryTree2(startNode->children[i]->left, env, cmd);
                free_command(cmd);
            }
            if (i == 0 && startNode->children[i]->right) 
            {
                command *cmd = init_command(dup(STDOUT_FILENO), dup(STDIN_FILENO));
                processBinaryTree2(startNode->children[i]->right, env, cmd);
                free_command(cmd);
            }
        }
    }
}

// Fonction principale pour exécuter le nœud de départ
void executer(StartNode* startNode, char **env) {
    expandCommandTrees2(startNode, env);
}