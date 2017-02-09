#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

volatile int intCount = 0;
volatile int tstpCount = 0;

void manejador(int signo) {
	if (signo == SIGINT)
		intCount++;
	if (signo == SIGTSTP)
		tstpCount++;
}

int main() {

	struct sigaction handler;

	if (sigaction(SIGINT, NULL, &handler) == -1) {
		perror("Unable to get SIGINT handler");
		return -1;
	}

	handler.sa_handler = manejador;

	if (sigaction(SIGINT, &handler, NULL) == -1) {
		perror("Unable to set SIGINT handler");
		return -1;
	}

	if (sigaction(SIGTSTP, NULL, &handler) == -1) {
		perror("Unable to get SIGTSTP handler");
		return -1;
	}

	handler.sa_handler = manejador;

	if (sigaction(SIGTSTP, &handler, NULL) == -1) {
		perror("Unable to set SIGTSTP handler");
		return -1;
	}

	sigset_t mascara;
	sigemptyset(&mascara);

	while (intCount + tstpCount < 10)
		sigsuspend(&mascara);

	printf("SIGINT captured: %i\n", intCount);
	printf("SIGTSTP captured: %i\n", tstpCount);

	return 0;
}
