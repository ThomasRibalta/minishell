#include "src/header/minishell.h"

int global_sig;

int	main(int ac, char **av, char **env)
{
    int i;
    init_signal();
    init_terminal(env);
    char **env2;
    
    if (ac > 2 && !*av)
	   return 0;

    env2 = clone_env(env);
    global_sig = 0;
    i = 0;
    start_terminal(env2, i);
    return 0;
}
