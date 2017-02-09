#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv) {

	if (argc < 2)
		return -1;

	int fd = open("./tuberia", O_WRONLY);
	if (fd == -1) {
		perror("Unable to open the pipe");
		close(fd);
		return -1;
	}

	size_t arglen = strlen(argv[1]);
	ssize_t wr = write(fd, argv[1], arglen);
	if (wr == -1) {
		perror("Unable to write on the pipe");
		close(fd);
		return -1;
	}

	close(fd);
	return 0;
}

