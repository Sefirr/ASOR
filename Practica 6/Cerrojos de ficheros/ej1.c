#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>

int main(int argc, char **argv) {

	int fd;

	struct flock fl,savelock;
	
	if((fd = open(argv[1], O_CREAT | O_RDWR, 0666) == -1)) {
		perror("open()");
		exit(EXIT_FAILURE);
	}

	

	return 0;
}
