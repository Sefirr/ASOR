#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
	
	if(pathconf(".",_PC_LINK_MAX) == -1) {
		perror("pathconf('.',_PC_LINK_MAX");
		exit -1;
	}else {
		printf("Numero máximo de enlaces: %ld\n", pathconf(".",_PC_LINK_MAX));      
	}
	
	if(pathconf(".",_PC_PATH_MAX) == -1) {
                perror("patchconf('.',_PC_PATH_MAX");
                exit -1;
        } else {
		printf("Tamaño máximo de ruta: %ld\n", pathconf(".",_PC_PATH_MAX)); 
	}

	if(pathconf(".",_PC_NAME_MAX) == -1) {
                perror("pathconf('.',_PC_NAME_MAX)");
                exit -1;
        } else {
		printf("Tamaño máximo de nombre de fichero: %ld\n", pathconf(".",_PC_NAME_MAX));
	}

	return 0;
}
