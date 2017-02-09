#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(int argc, char **argv) {
	if (argc < 2) {
		printf("Faltan argumentos, uso: ./ej3 program [args]\n"); 
		return -1;
	}

	pid_t pid = fork();
	pid_t nsid;
	int status;
	switch (pid) {
		case -1:
			perror("Unable to fork");
			return -1;
		break;

		// Demonized child
		case 0:
			nsid = setsid();
			if (nsid == -1) {
				perror("Unable to create a new session");
				exit(-1);
			}

			int ncwd = chdir("/tmp");
			if (ncwd == -1) {
				perror("Unable to change the CWD");
				exit(-1);
			}

			int daemon_in = open("/dev/null", O_RDONLY);
			int daemon_out = open("daemon.out", O_WRONLY | O_CREAT);
			int daemon_err = open("daemon.err", O_WRONLY | O_CREAT);
			if (daemon_in == -1 || daemon_out == -1 || daemon_err == -1) {
				perror("Failed to open in/out/err file:");
				close(daemon_in);
				close(daemon_out);
				close(daemon_err);
				exit(-1);
			}

			int dup_in = dup2(daemon_in, 0);
			if (dup_in == -1) {
				perror("Unable to duplicate the stdin:");
				exit(-1);
			}
			
			int dup_out = dup2(daemon_out, 1);
			if (dup_out == -1) {
				perror("Unable to duplicate the stdout:");
				exit(-1);
			}

			int dup_err = dup2(daemon_err, 2);
			if (dup_err == -1) {
				perror("Unable to duplicate the error stderr:");
				exit(-1);
			}

			int rc = execvp(argv[1], argv + 1);
			if (rc == -1) {
				perror("System call failed");
				exit(-1);
			}

			exit(0);
		break;
		default:
			wait(&status);
			if (status == -1) {
				perror("Command failed");
				return -1;
			}
			printf("El comando termino de ejecutarse\n");
			return 0;
		break;
	}
}

