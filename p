#include "src/header/minishell.h"

int	main(int ac, char **av, char **env)
{
    init_signal();
    init_terminal(env);
    char **env2;

    env2 = clone_env(env);
    start_terminal(ac, av, env2);
    return 0;
}
#include "src/header/minishell.h"

int	main(int ac, char **av, char **env)
{
    init_signal();
    init_terminal(env);
    char **env2;

    env2 = clone_env(env);
    start_terminal(ac, av, env2);
    return 0;
}
