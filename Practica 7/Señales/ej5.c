#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

volatile int stop = 0;

void manejador(int signo) {
	if (signo == SIGUSR1)
		stop = 1;
}

int main(int argc, char **argv) {

	if (argc != 2) {
		printf("Use: program secs\n");
		return -1;
	}

	sigset_t mask;
	sigemptyset(&mask);
	sigaddset(&mask, SIGUSR1);

	if (sigprocmask(SIG_UNBLOCK, &mask, NULL)) {	
		perror("Unable to unblock SIGUSR1");
		return -1;
	}

	struct sigaction handler;

	if (sigaction(SIGUSR1, NULL, &handler) == -1) {
		perror("Unable to get SIGUSR1 handler");
		return -1;
	}

	handler.sa_handler = manejador;

	if (sigaction(SIGUSR1, &handler, NULL) == -1) {
		perror("Unable to set SIGUSR1 handler");
		return -1;
	}

	sleep(atoi(argv[1]));

	if (stop == 1) {
		printf("Auto-destruction stopped\n");
		return 0;
	}

	if (unlink(argv[0]) == -1) {
		perror("Unable to delete myself");
		return -1;
	}
	else
		printf("Deleted myself\n");

	exit(0);
	return 0;
}
