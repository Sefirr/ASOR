#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>

int main() {

	if (-1 == mkfifo("tuberia1", 0644)) {
		perror("Unable to create the first pipe");
		return -1;
	}

	if (-1 == mkfifo("tuberia2", 0644)) {
		perror("Unable to create the second pipe");
		return -1;
	}

	char buffer[257];
	int nbuff = 0;

	int pipe1 = open("tuberia1", O_RDONLY | O_NONBLOCK);
	if (pipe1 == -1) {
		perror("Unable to open the first pipe");
		close(pipe1);
		return -1;
	}

	int pipe2 = open("tuberia2", O_RDONLY | O_NONBLOCK);
	if (pipe2 == -1) {
		perror("Unable to open the second pipe");
		close(pipe1);
		close(pipe2);
		return -1;
	}

	int selection, curpipe;
	do {
		fd_set pipes;
		FD_ZERO(&pipes);
		FD_SET(pipe1, &pipes);
		FD_SET(pipe2, &pipes);

		selection = select((pipe1 < pipe2) ? pipe2 + 1 : pipe1 + 1, &pipes, NULL, NULL, NULL);

		if (selection > 0) {
	
			int ntub = 0;
			if (FD_ISSET(pipe1, &pipes)) {
				ntub = 1;
				curpipe = pipe1;
			}
			else if (FD_ISSET(pipe2, &pipes)) {
				ntub = 2;
				curpipe = pipe2;
			}

			ssize_t readsize = 256;
			while (readsize == 256) {

				readsize = read(curpipe, buffer, 256);

				if (readsize == -1) {
					perror("Unable to read");
					close(pipe1);
					close(pipe2);
					return -1;
				}

				buffer[readsize] = '\0';
				printf("Tuberia %i: %s", ntub, buffer);
			}

			if (readsize != 256 && ntub == 1) {
				close(pipe1);
				pipe1 = open("tuberia1", O_RDONLY | O_NONBLOCK);

				if (pipe1 == -1) {
					perror("Unable to open the second pipe");
					close(pipe1);
					close(pipe2);
					return -1;
				}
			}

			else if (readsize != 256 && ntub == 2) {
				close(pipe2);
				pipe2 = open("tuberia2", O_RDONLY | O_NONBLOCK);

				if (pipe2 == -1) {
					perror("Unable to open the second pipe");
					close(pipe1);
					close(pipe2);
					return -1;
				}
			}
		}

	} while (selection != -1);

	perror("Error in the selection");
	close(pipe1);
	close(pipe2);
	return -1;
}
