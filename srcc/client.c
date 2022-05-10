/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afuchs <alexis.t.fuchs@gmail.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 14:07:17 by afuchs            #+#    #+#             */
/*   Updated: 2022/05/11 01:37:48 by afuchs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minitalk.h"

static void	send_char(pid_t pid, unsigned char c, char bit, char *cpid)
{
	if (bit < 8)
	{
		if (c % 2 == 0)
		{
			send_char(pid, c / 2, bit + 1, cpid);
			if (!cpid)
				usleep(100);
			else if (!sleep(2))
				ft_printf("Connection timed out\n");
			kill(pid, SIGUSR1);
		}
		else
		{
			send_char(pid, c / 2, bit + 1, cpid);
			if (!cpid)
				usleep(100);
			else if (!sleep(2))
				ft_printf("Connection timed out\n");
			kill(pid, SIGUSR2);
		}
	}
}

static void	signal_recieved(int sig)
{
	if (sig == SIGUSR2)
		ft_printf("Server confirmed !\n");
}

static void	init_sigstruct(struct sigaction *act)
{
	ft_bzero(act, sizeof (struct sigaction));
	sigemptyset(&(act->sa_mask));
	sigaddset(&(act->sa_mask), SIGUSR1);
	sigaddset(&(act->sa_mask), SIGUSR2);
	act->sa_handler = &signal_recieved;
	sigaction(SIGUSR1, act, (void *)0);
	sigaction(SIGUSR2, act, (void *)0);
}

int	main(int argc, char **argv)
{
	size_t				i;
	pid_t				pid;
	char				*cpid;
	struct sigaction	act;

	if (argc != 3)
		return (1);
	pid = ft_atoi(*(argv + 1));
	cpid = ft_itoa(getpid());
	init_sigstruct(&act);
	if (!cpid)
		ft_putstr_fd("Malloc error\n", 2);
	i = 0;
	while (*(cpid + i))
		send_char(pid, (unsigned char)*(cpid + i++), 0, (void *)0);
	send_char(pid, '\0', 0, (void *)0);
	i = 0;
	while (*(*(argv + 2) + i))
		send_char(pid, (unsigned char)*(*(argv + 2) + i++), 0, cpid);
	send_char(pid, '\0', 0, cpid);
	free(cpid);
	if (!sleep(2))
		ft_printf("Server did not respond...\n");
	return (0);
}
