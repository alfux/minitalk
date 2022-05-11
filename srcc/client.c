/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afuchs <alexis.t.fuchs@gmail.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 14:07:17 by afuchs            #+#    #+#             */
/*   Updated: 2022/05/11 13:56:36 by afuchs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minitalk.h"

static void	send_bit(pid_t pid, unsigned char c)
{
	if (!(c % 2))
		kill(pid, SIGUSR1);
	else
		kill(pid, SIGUSR2);
	pause();
}

static void	send_to_serv(pid_t pid, char *str)
{
	size_t	i;

	i = 0;
	while (*(str + i))
	{
		send_bit(pid, (unsigned char)*(str + i) >> 7);
		send_bit(pid, (unsigned char)*(str + i) >> 6);
		send_bit(pid, (unsigned char)*(str + i) >> 5);
		send_bit(pid, (unsigned char)*(str + i) >> 4);
		send_bit(pid, (unsigned char)*(str + i) >> 3);
		send_bit(pid, (unsigned char)*(str + i) >> 2);
		send_bit(pid, (unsigned char)*(str + i) >> 1);
		send_bit(pid, (unsigned char)*(str + i++));
	}
	i = 0;
	while (i++ < 8)
		send_bit(pid, '\0');
}

static void	sig_handle(int sig)
{
	if (sig == SIGUSR2)
	{
		ft_putstr_fd("\033[31mAn error occured\033[0m\n", 2);
		exit(EXIT_FAILURE);
	}
}

static void	init_sig(t_sig *act)
{
	ft_bzero(act, sizeof (t_sig));
	sigemptyset(&act->sa_mask);
	act->sa_handler = &sig_handle;
	sigaction(SIGUSR1, act, (void *)0);
	sigaction(SIGUSR2, act, (void *)0);
}

int	main(int argc, char **argv)
{
	t_sig	act;
	pid_t	pid;
	size_t	i;

	if (argc != 3)
		return (1);
	i = 0;
	while (*(*(argv + 1) + i))
		if (!ft_isdigit(*(*(argv + 1) + i++)))
			return (ft_putstr_l("\033[31mFirst parameter NaN\033[0m\n", 2));
	pid = ft_atoi(*(argv + 1));
	init_sig(&act);
	kill(pid, SIGUSR1);
	if (!(sleep(2)))
		return (ft_putstr_l("\033[31mServer does not respond\033[0m\n", 2));
	usleep(100);
	send_to_serv(pid, *(argv + 2));
	ft_printf("\033[32mServer confirmed reception !\033[0m\n");
	return (0);
}
