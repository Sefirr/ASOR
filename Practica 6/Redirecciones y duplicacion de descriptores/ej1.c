#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char **argv) {

	int fd = open(argv[1], O_CREAT | O_WRONLY | O_TRUNC, 0666);
	printf("Hola soy Manueh");
	
	//dup2(fd,1);
	close(1);
	dup(fd);
	return 0;

}
