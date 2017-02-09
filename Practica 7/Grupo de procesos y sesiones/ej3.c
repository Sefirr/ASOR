#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int showProcessInfo(char *id) {
	if (id == NULL)
		return -1;

	pid_t pid = getpid();
	gid_t gid = getgid();
	pid_t sid = getsid(pid);

	struct rlimit fileLimit;
	int rc = getrlimit(RLIMIT_NOFILE, &fileLimit);
	
	if (rc == -1) {
		perror("Unable to get the resource limits");
		return -1;
	}

	char *path = malloc(sizeof(char)*(4096 + 1));
	char *rpath = getcwd(path, 4096 + 1);

	if (rpath == NULL) {
		perror("Unable to get the CWD path");
		free(path);
		return -1;
	}

	printf("[%s] PID: %i\n", id, pid);
	printf("[%s] GID: %i\n", id, gid);
	printf("[%s] SID: %i\n", id, sid);
	printf("[%s] Max num of files: %ld\n", id, fileLimit.rlim_max);
	printf("[%s] CWD: %s\n", id, path);
		
	free (path);
	return 0;
}

int main() {
	pid_t pid = fork();
	pid_t nsid;
	
	switch (pid) {
		case -1:
			perror("Unable to fork");
			return -1;
		break;

		// Hijo
		case 0: 
			nsid = setsid();
			if (nsid == -1) {
				perror("Unable to create a new session");
				exit(-1);
			}

			if ((chdir("/tmp")) == -1) {
				perror("Unable to change the CWD");
				exit(-1);
			}

			showProcessInfo("Child");
			exit(0);
		break;

		// Padre
		default:
			showProcessInfo("Parent");
			int status;
			wait(&status);
			if (status == -1) {
				perror("Child process failed");
				return -1;
			}
	}

	return 0;
}
