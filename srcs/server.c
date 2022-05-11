/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afuchs <alexis.t.fuchs@gmail.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 14:08:41 by afuchs            #+#    #+#             */
/*   Updated: 2022/05/11 15:09:22 by afuchs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minitalk.h"

pid_t	cpid;

static void	init_sig(t_sig *act, void (*f)(int, siginfo_t *, void *))
{
	ft_bzero(act, sizeof (t_sig));
	sigemptyset(&act->sa_mask);
	act->sa_flags = SA_SIGINFO;
	act->sa_sigaction = f;
	sigaction(SIGUSR1, act, (void *)0);
	sigaction(SIGUSR2, act, (void *)0);
}

static void	first_sig(int sig, siginfo_t *info, void *ucontext)
{
	(void)sig;
	(void)ucontext;
	cpid = info->si_pid;
	kill(info->si_pid, SIGUSR1);
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
			cpid = 0;
		sig_handle(0, (void *)0, (void *)0);
	}
	else
		i++;
	kill(info->si_pid, SIGUSR1);
}

int	main(void)
{
	t_sig	act;

	ft_printf("\033[33mServer PID: %i\033[0m\n", getpid());
	init_sig(&act, &first_sig);
	while (1)
	{
		if (act.sa_sigaction == &first_sig)
		{
			pause();
			init_sig(&act, &sig_handle);
		}
		else if (!cpid)
			init_sig(&act, &first_sig);
		else if (!sleep(2))
		{
			ft_putstr_fd("\033[31mSignal lost\n\033[0m", 2);
			sig_handle(0, (void *)0, (void *)0);
			kill(cpid, SIGUSR2);
			init_sig(&act, &first_sig);
		}
	}
	return (0);
}
