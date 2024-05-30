/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 14:48:42 by ssoeno            #+#    #+#             */
/*   Updated: 2024/05/29 19:26:53 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include "./ft_printf/ft_printf.h"
#define INPUT_ERROR 1
#define PID_ERROR 2
#define KILL_ERROR 3
#define RESPONSE_ERROR 4

void	signal_handler(int signum, siginfo_t *info, void *context)
{
	static int				i = 7;
	static unsigned char	c;

	(void)context;
	(void)info;
	if (signum == SIGUSR1)
		c |= (1 << i);
	i--;
	if (i < 0)
	{
		write(1, &c, 1);
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
