#include "../header/minishell.h"

void  write_terminal_title()
{
  printf("\033[2J");
  printf("\033[1;1H"VIOLET " __  __  _____  _   _  _____   "VIOLET_FONCE
                "_____  _    _  ______  _       _\n" RESET
         VIOLET "|  \\/  ||_   _|| \\ | ||_   _| "VIOLET_FONCE
                "/ ____|| |  | ||  ____|| |     | |\n" RESET
         VIOLET "| \\  / |  | |  |  \\| |  | |  "VIOLET_FONCE
         "| (___  | |__| || |__   | |     | |\n" RESET
         VIOLET "| |\\/| |  | |  | . ` |  | |  "VIOLET_FONCE
         " \\___ \\ |  __  ||  __|  | |     | |\n" RESET
         VIOLET "| |  | | _| |_ | |\\  | _| |_  "VIOLET_FONCE
         "____) || |  | || |____ | |____ | |____\n" RESET
         VIOLET "|_|  |_||_____||_| \\_||_____|"VIOLET_FONCE
         "|_____/ |_|  |_||______||______||______|\n\n" RESET);
}

void  edit_shlvl(char **env)
{
    while (*env) 
    {
        if (ft_strncmp(*env, "SHLVL=", 6) == 0) 
        {
            ft_strcpy(*env + 6,ft_itoa(ft_atoi(*env + 6) + 1));
            break;
        }
        env++;
    }
}

char **clone_env(char **env) {
    size_t num_elements = 0;
    while (env[num_elements] != NULL) {
        num_elements++;
    }
    char **env_clone = malloc((num_elements + 1) * sizeof(char *));
    for (size_t i = 0; i < num_elements; i++) {
        env_clone[i] = strdup(env[i]);
    }
    env_clone[num_elements] = NULL;
    return env_clone;
}

void  init_terminal(char **env)
{
  edit_shlvl(env);
  write_terminal_title();
}


void start_terminal(int ac, char **av, char **env)
{
    char *input;
    
    while (1) 
    {
        input = readline(ft_strjoin(ft_strjoin(VERT"→ "BLEU, get_cwd(1)), VIOLET" > "RESET));
        if (!input) 
        {
            printf(VIOLET"\n\n FERMETURE DU TERMINAL, A LA PROCHAINE !! \n\n");
            exit(1);
        }
        add_history(input);
        check_prompt(input, &env);
    }
}