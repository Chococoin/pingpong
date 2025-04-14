#define _XOPEN_SOURCE 700
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pid_t ping_pid;
volatile sig_atomic_t running = 1;

int usleep(useconds_t usec);

void handle_sigusr1(int sig)
{
	(void)sig;
	write(1, "Pong 🏓\n", 7);
	kill(ping_pid, SIGUSR2);
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
	signal(SIGUSR1, handle_sigusr1);
	signal(SIGINT, handle_sigint);
	while (running)
		pause();
	return (0);
}
