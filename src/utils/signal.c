/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 14:18:07 by toto              #+#    #+#             */
/*   Updated: 2024/06/08 14:18:08 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

void	init_mask(struct sigaction *sig)
{
	sigemptyset(&sig->sa_mask);
	sigaddset(&sig->sa_mask, SIGINT);
	sigaddset(&sig->sa_mask, SIGQUIT);
}

void	signal_handler(int signum, siginfo_t *siginfo, void *context)
{
	char	*make_readline;

	(void)siginfo;
	(void)context;
	if (signum == SIGINT && g_global_sig == 0)
	{
		rl_on_new_line();
    		rl_replace_line("", 0);
    		rl_redisplay();
		make_readline = ft_strjoin(ft_strjoin(ft_strdup(VERT "→ " BLEU),
					get_cwd(1)), ft_strdup(VIOLET " > " RESET));
		write(1, "\n", 2);
		write(1, make_readline, ft_strlen(make_readline));
		free(make_readline);
	}
	else if (signum == SIGINT && g_global_sig == 1)
	{
		write(1, "\n", 2);
	}
	else
		return ;
}

void	init_sigaction(struct sigaction *sig)
{
	sigaction(SIGINT, sig, NULL);
	sig->sa_handler = SIG_IGN;
	sigaction(SIGQUIT, sig, NULL);
}

void	init_signal(void)
{
	struct sigaction	sig;

	ft_memset(&sig, 0, sizeof(struct sigaction));
	init_mask(&sig);
	sig.sa_flags = 0;
	sig.sa_sigaction = &signal_handler;
	init_sigaction(&sig);
}
