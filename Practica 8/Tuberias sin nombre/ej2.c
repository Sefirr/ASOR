#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

int main() {

	int p_h[2];
	int h_p[2];

	if (-1 == pipe(p_h) | -1 == pipe(h_p)) {
		perror("Unable to create the pipe");
		return -1;
	}

	pid_t pid = fork();
	switch (pid) {
		case -1:
			perror("Unable to fork");
			exit(-1);
		break;

		case 0:
			close(p_h[1]);
			close(h_p[0]);

			char recv_msg[257];
			char parent_msg[1] = {'l'};

			int i;
			for (i = 0; i < 10; i++) {
				ssize_t c_size = read(p_h[0], recv_msg, 256);
				if (c_size == -1) {
					perror("[Child] Error while reading from the pipe");
					exit(-1);
				}
				recv_msg[c_size] = '\0';

				printf("[Child] Message: %s", recv_msg);
				sleep(1);

				if (i == 9)
					parent_msg[0] = 'q';

				c_size = write(h_p[1], parent_msg, 1);
				if (c_size == -1) {
					perror("[Child] Error while wrinting into the pipe");
					exit(-1);
				}
			}

			close(p_h[0]);
			close(h_p[1]);
			exit(0);
		break;

		default:
			close(p_h[0]);
			close(h_p[1]);

			char user_msg[257];
			char child_msg[1] = {'l'};
			
			while (child_msg[0] != 'q') {
				printf("[Parent] Enter a new message:\n");

				ssize_t p_size = read(0, user_msg, 256);
				if (p_size == -1) {
					perror("[Parent] Error while reading from stdin");
					exit(-1);
				}

				user_msg[p_size] = '\0';
				p_size = write(p_h[1], user_msg, p_size + 1);
				if (p_size == -1) {
					perror("[Parent] Error while wrinting into the pipe");
					exit(-1);
				}

				while (child_msg[0] != 'l' && child_msg[0] != 'q') {
					p_size = read(h_p[0], child_msg, 1);
					if (p_size == -1) {
						perror("[Parent] Error while reading from the pipe");
						exit(-1);
					}
				}
			}

			close(p_h[1]);
			close(h_p[0]);
			exit(0);
	}

	return 0;
}
