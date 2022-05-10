/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afuchs <alexis.t.fuchs@gmail.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 14:08:41 by afuchs            #+#    #+#             */
/*   Updated: 2022/05/11 01:35:12 by afuchs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minitalk.h"

pid_t	g_cpid;

static void	print_lst(t_list **sentence)
{
	char	*str;
	t_list	*next;
	size_t	i;

	str = ft_calloc(ft_lstsize(*sentence), sizeof (char));
	i = 0;
	next = *sentence;
	while (next)
	{
		*(str + i++) = *((char *)next->content);
		next = next->next;
	}
	if (g_cpid)
	{
		ft_printf("%s\n", str);
		kill(g_cpid, SIGUSR2);
		g_cpid = 0;
	}
	else
		g_cpid = ft_atoi(str);
	free(str);
	ft_lstclear(sentence, &free);
	if (g_cpid)
		usleep(100);
}

static void	write_bit(char *octet, int sign, size_t i)
{
	if (sign == SIGUSR1)
		*octet = *octet | 0 << (7 - (i % 8));
	else
		*octet = *octet | 1 << (7 - (i % 8));
}

static void	catch_sentence(int sign)
{
	static size_t	i;
	static t_list	*sentence;
	static t_list	*new;

	if (i % 8 == 0)
	{
		new = ft_lstnew(ft_calloc(1, sizeof (char)));
		if (!i)
			sentence = new;
		else
			(*ft_lstlast(sentence)).next = new;
	}
	write_bit((char *)new->content, sign, i);
	if (i && (i % 8) == 7 && *((char *)new->content) == '\0')
	{
		print_lst(&sentence);
		new = (void *)0;
		i = 0;
	}
	else
		i++;
	if (g_cpid)
		kill(g_cpid, SIGUSR1);
}

int	main(void)
{
	sigset_t			set;
	struct sigaction	act;

	ft_printf("SERVER PID: %i\n", getpid());
	ft_bzero(&act, sizeof (struct sigaction));
	sigemptyset(&set);
	sigaddset(&set, SIGUSR1);
	sigaddset(&set, SIGUSR2);
	act.sa_handler = &catch_sentence;
	act.sa_mask = set;
	sigaction(SIGUSR1, &act, (void *)0);
	sigaction(SIGUSR2, &act, (void *)0);
	while (1)
		pause();
	return (0);
}
