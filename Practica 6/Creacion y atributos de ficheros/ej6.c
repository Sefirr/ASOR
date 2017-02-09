#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	mode_t cmask;
	cmask = umask(0027);
	int fd;

	if((fd = open(argv[1], O_CREAT, 0666)) == -1) {
		perror("open()");
		exit -1;
	}
	
	umask(cmask);


	return 0;
}
