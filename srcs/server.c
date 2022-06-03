/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afuchs <alexis.t.fuchs@gmail.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 14:08:41 by afuchs            #+#    #+#             */
/*   Updated: 2022/06/03 14:29:19 by afuchs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minitalk.h"

pid_t	g_cpid;

static void	init_sig(t_sig *act, void (*f)(int, siginfo_t *, void *))
{
	ft_bzero(act, sizeof (t_sig));
	sigemptyset(&act->sa_mask);
	sigaddset(&act->sa_mask, SIGUSR1);
	sigaddset(&act->sa_mask, SIGUSR2);
	act->sa_flags = SA_SIGINFO;
	act->sa_sigaction = f;
	sigaction(SIGUSR1, act, (void *)0);
	sigaction(SIGUSR2, act, (void *)0);
	if (g_cpid < 0)
		g_cpid *= -1;
}

static void	first_sig(int sig, siginfo_t *info, void *ucontext)
{
	(void)sig;
	(void)ucontext;
	g_cpid = info->si_pid;
}

static void	sig_handle(int sig, siginfo_t *info, void *ucontext)
{
	static char	i;
	static char	c;

	if (!info && !ucontext)
	{
		i = 0;
		c = 0;
		return ;
	}
	if (sig == SIGUSR2)
		c = c | (1 << (7 - i));
	if (i == 7)
	{
		ft_putchar_fd(c, 1);
		if (c == '\0')
			g_cpid *= -1;
		sig_handle(0, (void *)0, (void *)0);
	}
	else
		i++;
}

static void	sendsig(t_sig *act)
{
	static char	i;

	kill(g_cpid, SIGUSR1);
	if (!usleep(100) && i < 10)
	{
		i++;
		sendsig(act);
	}
	else
		i = 0;
	if (i >= 10)
	{
		i = 0;
		ft_putstr_fd("\033[31mSignal lost\n\033[0m", 2);
		sig_handle(0, (void *)0, (void *)0);
		kill(g_cpid, SIGUSR2);
		init_sig(act, &first_sig);
		g_cpid = 0;
	}
}

int	main(void)
{
	t_sig	act;

	ft_printf("\033[33mServer PID: %i\033[0m\n", getpid());
	while (1)
	{
		if (act.sa_sigaction == &sig_handle && g_cpid > 0)
			sendsig(&act);
		else if (g_cpid <= 0)
		{
			init_sig(&act, &first_sig);
			if (g_cpid)
				kill(g_cpid, SIGUSR1);
			pause();
			init_sig(&act, &sig_handle);
		}
	}
	return (0);
}
