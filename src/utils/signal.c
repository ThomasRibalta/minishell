#include "../header/minishell.h"

void    init_mask(struct sigaction *sig)
{
    sigemptyset(&sig->sa_mask);
    sigaddset(&sig->sa_mask, SIGINT);
    sigaddset(&sig->sa_mask, SIGQUIT);
}

void signal_handler(int signum, siginfo_t *siginfo, void *context) 
{
	(void) siginfo;
	(void) context;
    if (signum == SIGINT && global_sig == 0)
    {
        char *make_readline = ft_strjoin(ft_strjoin(ft_strdup(VERT"→ "BLEU), get_cwd(1)), ft_strdup(VIOLET" > "RESET));
        write(1,"\n", 2);
        write(1, make_readline, ft_strlen(make_readline));
        free(make_readline);
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
