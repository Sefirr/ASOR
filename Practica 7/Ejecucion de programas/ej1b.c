#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char **argv) {
	if (argc < 2) {
		printf("Usage: ./ej1b command [args]\n"); 
		return -1;
	}

	int rc = execvp(argv[1], argv + 1);
	if (rc == -1) {
		perror("execvp()");
		exit(-1);
	}		
	
	printf("El comando termino de ejecutarse\n");
	return 0;

}
