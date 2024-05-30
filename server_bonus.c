/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 14:48:42 by ssoeno            #+#    #+#             */
/*   Updated: 2024/05/29 19:26:37 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include "./ft_printf/ft_printf.h"
#define INPUT_ERROR 1
#define PID_ERROR 2
#define KILL_ERROR 3
#define RESPONSE_ERROR 4

static void	error_handling(int errno)
{
	if (errno == INPUT_ERROR)
		ft_printf("Usage: ./client [PID] [string]");
	if (errno == PID_ERROR)
		ft_printf("PID is invalid");
	if (errno == KILL_ERROR)
		ft_printf("Kill command error");
	exit(EXIT_FAILURE);
}

void	signal_handler(int signum, siginfo_t *info, void *context)
{
	static int				i = 7;
	static unsigned char	c;

	(void)context;
	if (signum == SIGUSR1)
		c |= (1 << i);
	i--;
	if (i < 0)
	{
		write(1, &c, 1);
		if (c == '\0')
		{
			if (kill(info->si_pid, SIGUSR2) == -1)
				error_handling(KILL_ERROR);
		}
		c = 0;
		i = 7;
	}
}

int	main(void)
{
	struct sigaction	sa;

	sa.sa_sigaction = signal_handler;
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	ft_printf("PID: %d\n", getpid());
	if (sigaction(SIGUSR1, &sa, NULL) == -1
		|| sigaction(SIGUSR2, &sa, NULL) == -1)
	{
		ft_printf("Failed to set signal handler");
		exit(EXIT_FAILURE);
	}
	while (1)
		pause();
	return (0);
}
