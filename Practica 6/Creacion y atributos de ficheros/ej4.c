#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char **argv) {

	int fd = open(argv[1], O_CREAT, 0645);


	return 0;
}
