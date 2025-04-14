#define _XOPEN_SOURCE 700
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pid_t	pong_pid;
volatile sig_atomic_t running = 0;

int usleep(useconds_t usec);

void	handle_sigusr2(int sig)
{
	(void)sig;
	write(1, "Ping 🏓\n", 7);
	kill(pong_pid, SIGUSR1);
	usleep(500000);
}

void	handle_sigint(int sig)
{
	(void)sig;
	write(1, "\n Terminando... \n", 17);
	kill(pong_pid, SIGINT);
	running = 0;
}

int	main(void)
{
	signal(SIGUSR2, handle_sigusr2);
	signal(SIGUSR1, handle_sigint);
	printf("Ping listo a servir! PID: %d\n", getpid());
	printf("Esperando pong... \n");
	while (running)
		pause();
	return (0);
}
