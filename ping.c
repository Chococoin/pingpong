#define _XOPEN_SOURCE 700
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pid_t	pong_pid;
volatile sig_atomic_t running = 1;

int usleep(useconds_t usec);

void	handle_sigusr1	(int sig, siginfo_t *info, void *context)
{
	(void)sig;
	(void)context;
	pong_pid = info->si_pid;
	write(1, "Ping 🏓\n", 7);
	kill(pong_pid, SIGUSR2);
	usleep(500000);
}

void handle_sigusr2(int sig, siginfo_t *info, void *context)
{
    (void)sig;
    (void)context;
    (void)info; // Added to silence unused parameter warning
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
	exit(0);
}

int	main(void)
{
	struct sigaction sa;
	sa.sa_sigaction = handle_sigusr1;
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGUSR1, &sa, NULL);
	signal(SIGINT, handle_sigint);
	struct sigaction sb;
	sb.sa_sigaction = handle_sigusr2;
	sb.sa_flags = SA_SIGINFO;
	sigemptyset(&sb.sa_mask);
	sigaction(SIGUSR2, &sb, NULL);
	printf("Ping listo a servir! PID: %d\n", getpid());
	printf("Esperando pong... \n");
	kill(pong_pid,SIGUSR1);
	while (running)
		pause();
	return (0);
}
