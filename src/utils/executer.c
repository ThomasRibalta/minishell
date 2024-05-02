#include "../header/minishell.h"

#include <string.h>

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
        !ft_strcmp(value, "pwd") ||
        !ft_strcmp(value, "unset") ||
        !ft_strcmp(value, "export") ||
        !ft_strcmp(value, "echo") ||
        !ft_strcmp(value, "env") ||
        !ft_strcmp(value, "exit")) {
        return 1;
    }
    return 0;
}

char	*get_path(char **env)
{
	while (ft_strncmp("PATH=", *env, 5) != 0 && *env && env)
		env++;
	return (*env + 5);
}

char	*check_path(char **paths, char *name)
{
	char	*join;
	char	*tmp;
	int		i;

	i = 0;
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

void	free_all(char **tab)
{
	char	**tmp;

	tmp = tab;
	while (*tab)
	{
		free(*tab);
		tab++;
	}
	free(tmp);
}

void	execute(char **param, char *path, char **env)
{
	char	**paths;
	char	*path1;

	paths = ft_split(path, ':');
	path1 = check_path(paths, clean_quote(param[0]));
	if (!path1)
	{
		//free_all(paths);
		//free_all(param);
	}
	if (execve(path1, param, env) == -1)
	{
		//free_all(paths);
		//free_all(param);
	}
	//free_all(paths);
	//free_all(param);
	//free(path1);
}

void exec(ASTNode* node, char **env, int test, int test2) {
    char **split_nodeValue;
    int		p_id[2];
    pid_t pid;

    if (node == NULL || node->value == NULL) {
        return ;
    }
    if (pipe(p_id) == -1)
		    exit(0);
    split_nodeValue = ft_split(node->value, ' ');
    if (is_builtin(clean_quote(split_nodeValue[0])))
      write(1,"test\n", 5);
    pid = fork();
    if (pid == -1) {
        perror("fork");
        return;
    }
    if (pid == 0) {
      if (!(node->is_last_command))
      {
        dup2(p_id[1], 1);
        close(p_id[0]);
      }else
      {
        dup2(test, 1);
        close(test);
      }
      execute(split_nodeValue, get_path(env), env);
    } else {
      if (!(node->is_last_command))
      {
        close(p_id[1]);
        dup2(p_id[0], 0);
      }else
      {
        dup2(test2, STDIN_FILENO);
        close(test2);
      }
    }
}


void processBinaryTree2(ASTNode* node, char **env, int test, int test2) {
    if (node == NULL) return;
    processBinaryTree2(node->left, env, test, test2);
    if (node->type == NODE_COMMAND) {
        exec(node, env, test, test2);
    }
    processBinaryTree2(node->right, env, test, test2);
}

void expandCommandTrees2(StartNode* startNode, char **env) {
    if (!startNode->hasLogical) {
        int test = dup(STDOUT_FILENO);
        int test2 = dup(STDIN_FILENO);
        processBinaryTree2(startNode->children[0]->left, env, test, test2);
        wait(NULL);
    } else {
      int test, test2 = 1;
        for (int i = 0; i < startNode->childCount; i++) {
            if (startNode->children[i]->left) {
                processBinaryTree2(startNode->children[i]->left, env, test, test2);
            }
            if (i == 0 && startNode->children[i]->right) {
                processBinaryTree2(startNode->children[i]->right, env, test, test2);
            }
        }
    }
}

void executer(StartNode* startNode, char **env)
{
	expandCommandTrees2(startNode, env);
}