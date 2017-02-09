#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char **argv) {
	
	
	//argv[1] = comando argv[2-n] = parametros
	if(argc < 2)	{
		printf("Usage: command <argv1> <argv2> ...");
		exit(EXIT_FAILURE);
	}
	int i;
	char command[255];
	strcpy(command, argv[1]);
	for(i= 2; i < argc; i++) {
		strcat(command, " ");
		strcat(command,argv[i]);
	}
	
	system(command);

	return 0;

}
