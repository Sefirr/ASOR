#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

int main() {

	sigset_t mascara;

	sigemptyset(&mascara);
	sigaddset(&mascara, SIGINT);
	sigaddset(&mascara, SIGTSTP);

	if (sigprocmask(SIG_BLOCK, &mascara, NULL) == -1) {
		perror("Unable to block the signals");
		return -1;
	}

	char *sleep_secs = getenv("SLEEP_SECS");
	if (sleep_secs == NULL) {
		printf("Unable to read SLEEP_SECS\n");
		return -1;
	}

	int secs = atoi(sleep_secs);
	if (secs < 0)
		secs = 0;

	sleep(secs);

	sigset_t pendiente;
	sigemptyset(&pendiente);
	if (sigpending(&pendiente) == -1) {
		perror("Unable to check the pending signals");
		return -1;
	}

	switch (sigismember(&pendiente, SIGINT)) {
		case 1:
			printf("SIGINT recieved during sleep\n");
		break;

		case -1:
			perror("Unable to check the pending signals");
			return -1;
		break;
	}

	switch (sigismember(&pendiente, SIGTSTP)) {
		case 1:
			printf("SIGTSTP recieved during sleep, unblocking signal...\n");
			sigdelset(&mascara, SIGINT);
			if (sigprocmask(SIG_UNBLOCK, &mascara, NULL) == -1) {
				perror("Unable to block the signals");
				return -1;
			}
		break;

		case -1:
			perror("Unable to check the pending signals");
			return -1;
		break;
	}

	printf("End of the program\n");	

	return 0;
}
