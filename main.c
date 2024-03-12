#include "src/header/minishell.h"

int	main(int ac, char **av, char **env)
{
    init_signal();
    init_terminal(env);
    start_terminal(ac, av, env);
    return 0;
}
