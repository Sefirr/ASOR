#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char **argv) {

	int fd = open(argv[1], O_CREAT | O_WRONLY | O_APPEND , 0666);

	close(1);
	//dup(fd);
	close(2);
	dup2(fd, 1);
	dup2(fd, 2);

	printf("Hola soy Manueh\n");
	if(setuid(0) == -1) {
		perror("setuid()");
	}

	
	return 0;

}
