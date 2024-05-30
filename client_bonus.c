/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 14:48:16 by ssoeno            #+#    #+#             */
/*   Updated: 2024/05/29 17:38:11 by ssoeno           ###   ########.fr       */
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

static void	send_bit(pid_t s_pid, char c)
{
	int	sig;
	int	i;

	i = 7;
	while (i >= 0)
	{
		if (c >> i & 1)
			sig = SIGUSR1;
		else
			sig = SIGUSR2;
		if (kill(s_pid, sig) == -1)
			error_handling(KILL_ERROR);
		usleep(100);
		i--;
	}
}

static int	check_pid(char *c_pid)
{
	int	i_pid;

	i_pid = ft_atoi(c_pid);
	if (i_pid <= 1)
		error_handling(PID_ERROR);
	return (i_pid);
}

void	response_handler(int signum, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;
	if (signum == SIGUSR2)
	{
		write(1, "Message received by server.\n", 28);
		exit(EXIT_SUCCESS);
	}
}

int	main(int argc, char **argv)
{
	size_t				i;
	size_t				len;
	pid_t				i_pid;
	struct sigaction	sa;

	if (argc != 3)
		error_handling(INPUT_ERROR);
	i_pid = check_pid(argv[1]);
	len = ft_strlen(argv[2]);
	sa.sa_sigaction = response_handler;
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGUSR1, &sa, NULL) == -1
		|| sigaction(SIGUSR2, &sa, NULL) == -1)
		exit(EXIT_FAILURE);
	i = 0;
	while (i < len)
	{
		send_bit(i_pid, argv[2][i]);
		i++;
	}
	send_bit(i_pid, '\0');
	while (1)
		pause();
	return (EXIT_SUCCESS);
}