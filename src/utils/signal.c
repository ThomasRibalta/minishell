#include "../header/minishell.h"

void    init_mask(struct sigaction *sig)
{
    sigemptyset(&sig->sa_mask);
    sigaddset(&sig->sa_mask, SIGINT);
    sigaddset(&sig->sa_mask, SIGQUIT);
}

void signal_handler(int signum, siginfo_t *siginfo, void *context) 
{
    if (signum == SIGINT && global_sig == 0)
    {
        write(1,"\n", 2);
        write(1, ft_strjoin(ft_strjoin(VERT"→ "BLEU, get_cwd()), VIOLET" > "RESET), 
        ft_strlen(ft_strjoin(ft_strjoin(VERT"→ "BLEU, get_cwd()), VIOLET" > "RESET)));
    }
    else
        return ;
}

void init_sigaction(struct sigaction *sig)
{
    sigaction(SIGINT, sig, NULL);
    sigaction(SIGQUIT, sig, NULL);
}

void init_signal()
{
    struct sigaction sig;

    ft_memset(&sig, 0, sizeof(struct sigaction));
    init_mask(&sig);
    sig.sa_flags = 0;
    sig.sa_sigaction = &signal_handler;
    init_sigaction(&sig);
}