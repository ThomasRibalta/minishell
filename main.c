#include "src/header/minishell.h"

int global_sig;

void free_tab(char **tab)
{
    int i = 0;
    while (tab[i])
    {
        free(tab[i]);
        i++;
    }
    free(tab);
}

int	main(int ac, char **av, char **env)
{
    int i;
    init_signal();
    init_terminal(env);
    char **env2;
    char **export;
    
    if (ac > 2 && !*av)
	   return 0;
    env2 = clone_env(env);
    export = clone_env(env);
    global_sig = 0;
    i = 0;
    start_terminal(env2, export, i);
    free_tab(env2);
    free_tab(export);
    return 0;
}