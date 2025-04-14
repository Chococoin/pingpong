#define _XOPEN_SOURCE 700
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pid_t ping_pid;
volatile sig_atomic_t running = 1;

int usleep(useconds_t usec);

void handle_sigusr2(int sig, siginfo_t *info, void *context)
{
	(void)sig;
	(void)info;
	(void)context;
	if (ping_pid == 0)
		ping_pid = info->si_pid;
	write(1, "Pong 🏓\n", 7);
	kill(ping_pid, SIGUSR1);
	usleep(500000);
}

void handle_sigint(int sig)
{
	(void)sig;
	write(1, "\n [Pong]Terminando... \n", 23);
	kill(ping_pid, SIGINT);
	running = 0;
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		write(1, "Use PID of ping as arg\n", 23);
		exit(1);
	}
	ping_pid = atoi(argv[1]);
	struct sigaction sa;
	sa.sa_sigaction = handle_sigusr2;
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGUSR1, &sa, NULL);
	struct sigaction sb;
	sb.sa_sigaction = handle_sigusr2;
	sb.sa_flags = SA_SIGINFO;
	sigemptyset(&sb.sa_mask);
	sigaction(SIGUSR2, &sb, NULL);
	kill(ping_pid,SIGUSR1);
	while (running)
		pause();
	return (0);
}
