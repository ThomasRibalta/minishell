#include "../header/minishell.h"

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
    if (name[0] == '.' && name[1] == '/')
        return (name);
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
        printf("minishell: %s: command not found\n", param[0]);
        free_all(paths);
        exit(EXIT_FAILURE);
    }
    if (execve(path1, param, env) == -1) {
        free_all(paths);
    }
    free(path1);
    exit(EXIT_FAILURE);
}

int execute_builtin(char ***env, char **param){
    if (ft_strcmp(clean_quote(param[0]), "exit") == 0)
        exit_program(param[1]);
    else if (ft_strcmp(clean_quote(param[0]), "cd") == 0)
        cd(param + 1, env);
    else if (ft_strcmp(clean_quote(param[0]), "export") == 0 && param[1] != NULL)
        export_var(env, param + 1);
    else if (ft_strcmp(clean_quote(param[0]), "unset") == 0)
        unset_var(env, param + 1);
    else 
        return 1;
    return 0;
}

int execute_fork_builtin(char **env, char **param){
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

void handle_child_process(ASTNode *node, command *cmd, int p_id[2], char **split_nodeValue, char ***env, int fd) {
    if (!(node->is_last_command) || node->outputs != NULL || node->appends != NULL)
    {
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
        execute_fork_builtin(*env, split_nodeValue);
        exit(EXIT_SUCCESS);
    }
    execute(split_nodeValue, get_path(*env), *env);
}

void handle_parent_process(ASTNode *node, command *cmd, int p_id[2], pid_t pid) {
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

int get_number_of_files()
{
    int n;
    DIR *dir;
    struct dirent *entry;

    n = 0;
    dir = opendir(".");
    if (!dir)
    {
        perror("diropen");
        exit(EXIT_FAILURE);
    }
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_name[0] == '.' && (entry->d_name[1] == '\0' || (entry->d_name[1] == '.' && entry->d_name[2] == '\0'))) {
            continue;
        }
        n++;
    }
    closedir(dir);
    return n;
}

char **get_current_file()
{
    char **tab;
    int n;
    int i;
    DIR *dir;
    struct dirent *entry;

    n = get_number_of_files();
    i = 0;
    tab = malloc(sizeof(char *) * (n + 1));
    if (!tab)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    dir = opendir(".");
    if (!dir)
    {
        perror("diropen");
        exit(EXIT_FAILURE);
    }
    while ((entry = readdir(dir)) != NULL)
    {
        if ((entry->d_name[0] == '.' && (entry->d_name[1] == '\0' || (entry->d_name[1] == '.' && entry->d_name[2] == '\0'))) || entry->d_name[0] == '.') {
            continue;
        }
        tab[i] = strdup(entry->d_name);
        i++;
    }
    closedir(dir);
    tab[n] = NULL;
    return (tab);
}

int word_at_end(char *value1, char *value2) {
    int len1;
    len1 = strlen(value1);
    int len2;
    len2 = strlen(value2);

    if (len2 > len1)
        return 0;
    return (strcmp(value1 + len1 - len2, value2) == 0) ? 1 : 0;
}


int word_at_start(char *value1, char *value2) {
    int len2 = strlen(value2);

    return (strncmp(value1, value2, len2) == 0) ? 1 : 0;
}

int process_tab_current_file(char **tab_current_file, char **tab, int i, int pos, int j)
{
    int n = 0;
    int count = 0;

    while (tab_current_file[n])
    {
        if (pos == 1)
        {
            if (word_at_end(tab_current_file[n], tab[i] + 1))
                count++;
        }
        else if (pos == 2)
        {
            if (word_at_start(tab_current_file[n], tab[i] + 1) && ft_strlen(tab[i]) > 1)
                count++;
            else
                count++;
            
        }
        else if (pos == 3)
        {
            if (word_at_end(tab_current_file[n], tab[i] + j + 1) && word_at_start(tab_current_file[n], ft_substr(tab[i], 0, j)))
                count++;
        }
        n++;
    }
    return (count);
}

int tab_len(char **tab)
{
    int i = 0;

    while (tab[i])
    {
        i++;
    }
    return (i);
}

char **replace_wildcard(char **tab, int i, int pos, int j)
{
    char **tab_current_file;
    int n = 1;
    int m = 1;

    tab_current_file = get_current_file();
    int k = process_tab_current_file(tab_current_file, tab, i, pos, j);
    char **new_tab = malloc(sizeof(char *) * (k + 1 + tab_len(tab)));
    new_tab[0] = tab[0];
    while (tab[m])
    {
        if (m  == i && tab[m + 1])
            m++;
        else if (m == i && !tab[m + 1])
            break;
        new_tab[n] = strdup(tab[m]);
        n++;
        m++;
    }
    m = 0;
    while (tab_current_file[m])
    {
        if (pos == 1)
        {
            if (word_at_end(tab_current_file[m], tab[i] + 1))
            {
                new_tab[n] = strdup(tab_current_file[m]);
                n++;
            }
        }
        else if (pos == 2)
        {
            if (word_at_start(tab_current_file[m], tab[i] + 1) && ft_strlen(tab[m]) > 1)
            {
                new_tab[n] = strdup(tab_current_file[m]);
                n++;
            }
            else
            {
                new_tab[n] = strdup(tab_current_file[m]);
                n++;
            }
            
        }
        else if (pos == 3)
        {
            if (word_at_end(tab_current_file[m], tab[i] + j + 1) && word_at_start(tab_current_file[m], ft_substr(tab[i], 0, j)))
            {
                new_tab[n] = strdup(tab_current_file[m]);
                n++;
            }
        }
       m++;
    }
    new_tab[n] = NULL;
    return (new_tab);
}

char **check_wildcard(char **split_nodeValue)
{
    int i = 0;
    int j = 0;
    char **tab_current_file = split_nodeValue;

    while (split_nodeValue[i])
    {
        while(split_nodeValue[i][j])
        {
            if (split_nodeValue[i][j] == '*' && j == 0)
            {
                tab_current_file = replace_wildcard(split_nodeValue, i, 1, j);
                break;
            }
            else if (split_nodeValue[i][j] == '*' && split_nodeValue[i][j + 1] == '\n')
            {
                tab_current_file = replace_wildcard(split_nodeValue, i, 2, j);
                break;
            }
            else if (split_nodeValue[i][j] == '*')
            {
                tab_current_file = replace_wildcard(split_nodeValue, i, 3, j);
                break;
            }
            j++;
        }
        j = 0;
        i++;
    }
    return (tab_current_file);
}

void execute_command(ASTNode* node, char ***env, command *cmd, int fd, int *exit_status)
{
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
    replaceEnvVars(&node->value, *env, exit_status);
    split_nodeValue = ft_split(node->value, ' ');
    split_nodeValue = check_wildcard(split_nodeValue);
    if (is_builtin(clean_quote(split_nodeValue[0])))
    {
        execute_builtin(env, split_nodeValue);
        return;
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

void execute_parenthese(ASTNode* node, char ***env, int fd, command *cmd, int *exit_status) {
    int p_id[2];
    pid_t pid;

    if (pipe(p_id) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    if (fd == -1)
        fd = p_id[1];
    if (node->inputs)
        redirection_in(node->inputs->filename);
    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (pid == 0) {
        if (!(node->is_last_command) || node->outputs != NULL || node->appends != NULL)
        {
            dup2(fd, STDOUT_FILENO);
            close(p_id[0]);
        }
        else
        {
            dup2(cmd->std_out, STDOUT_FILENO);
            close(cmd->std_out);
        }
        lexer(ft_substr(node->value, 1, ft_strlen(node->value) - 2), env, exit_status);
        exit(EXIT_SUCCESS);
    } else {
        waitpid(pid, NULL, 0);
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
    }
}

void execute_output_append_command(ASTNode* node, char ***env, command *cmd, int *exit_status) {
    if (node->outputs){
        while (node->outputs) {
            int fd = open(node->outputs->filename, O_WRONLY | O_CREAT, 0644);
            if (fd == -1) {
                perror("open");
                exit(EXIT_FAILURE);
            }
            execute_command(node, env, cmd, fd, exit_status);
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
            execute_command(node, env, cmd, fd, exit_status);
            close(fd);
            node->appends = node->appends->next;
        }
    }
    if (!node->is_last_command)
        execute_command(node, env, cmd, -1, exit_status);
}

void execute_output_append_parenthese(ASTNode* node, char ***env, command *cmd, int *exit_status) {
    if (node->outputs) {
        while (node->outputs) {
            int fd = open(node->outputs->filename, O_WRONLY | O_CREAT, 0644);
            if (fd == -1) {
                perror("open");
                exit(EXIT_FAILURE);
            }
            execute_parenthese(node, env, fd, cmd, exit_status);
            close(fd);
            node->outputs = node->outputs->next;
        }
    }
    if (node->appends) {
        while (node->appends) {
            int fd = open(node->appends->filename, O_WRONLY | O_APPEND | O_CREAT, 0644);
            if (fd == -1) {
                perror("open");
                exit(EXIT_FAILURE);
            }
            execute_parenthese(node, env, fd, cmd, exit_status);
            close(fd);
            node->appends = node->appends->next;
        }
    }
    execute_parenthese(node, env, -1, cmd, exit_status);
}

void processBinaryTree2(ASTNode* node, char ***env, command *cmd, int *exit_status) {
    if (node == NULL) return;
    processBinaryTree2(node->left, env, cmd, exit_status);
    if (node->type == NODE_COMMAND) {
        if (node->outputs || node->appends)
            execute_output_append_command(node, env, cmd, exit_status);
        else
        {
            execute_command(node, env, cmd, -1, exit_status);
        }
    }
    if (node->type == NODE_PARENTHESE) {
        execute_output_append_parenthese(node, env, cmd, exit_status);
    }
    processBinaryTree2(node->right, env, cmd, exit_status);
}

command *init_command(int test, int test2) {
    command *cmd = malloc(sizeof(command));
    cmd->std_out = test;
    cmd->std_in = test2;
    cmd->pids = malloc(1024 * sizeof(int));
    cmd->pid_count = 0;
    return cmd;
}

void    free_command(command *cmd, int *exit_status)
{
    for (int j = 0; j < cmd->pid_count; j++)
        waitpid(cmd->pids[j], exit_status, 0);
    free(cmd->pids);
    free(cmd);
}

void expandCommandTrees2(StartNode* startNode, char ***env, int *exit_status) {
    if (!startNode->hasLogical) 
    {
        command *cmd = init_command(dup(STDOUT_FILENO), dup(STDIN_FILENO));
        processBinaryTree2(startNode->children[0]->left, env, cmd, exit_status);
        free_command(cmd, exit_status);
        //printf("exit_status: %d\n", MY_WEXITSTATUS(*exit_status));
    } 
    else 
    {
        for (int i = 0; i < startNode->childCount; i++) 
        {
            if (startNode->children[i]->left) 
            {
                command *cmd = init_command(dup(STDOUT_FILENO), dup(STDIN_FILENO));
                processBinaryTree2(startNode->children[i]->left, env, cmd, exit_status);
                free_command(cmd, exit_status);
            }
            if (i == 0 && startNode->children[i]->right) 
            {
                command *cmd = init_command(dup(STDOUT_FILENO), dup(STDIN_FILENO));
                processBinaryTree2(startNode->children[i]->right, env, cmd, exit_status);
                free_command(cmd, exit_status);
            }
        }
    }
}

// Fonction principale pour exécuter le nœud de départ
void executer(StartNode* startNode, char ***env, int *exit_status) {
    global_sig = 1;
    expandCommandTrees2(startNode, env, exit_status);
    global_sig = 0;
}
