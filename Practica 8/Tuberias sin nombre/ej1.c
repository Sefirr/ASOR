#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

int main(int argc, char **argv) {

	if (argc != 5) {
		perror("Invalid arguments");
		return -1;
	}

	int tub[2];
	if (pipe(tub) == -1) {
		perror("Unable to create the tub");
		return -1;
	}

	pid_t pid = fork();
	int nfd;

	switch (pid) {
		case -1:
			perror("Unable to fork");
			exit(-1);
		break;

		case 0:
			close(tub[1]);
			nfd = dup2(tub[0], 0);
			close(tub[0]);
			execlp(argv[3], argv[3], argv[4], NULL);
		break;
			
		default:
			close(tub[0]);
			nfd = dup2(tub[1], 1);
			close(tub[1]);
			execlp(argv[1], argv[1], argv[2], NULL);
	}

	return 0;
}
