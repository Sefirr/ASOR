#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int main() {

	int i;
	for(i=1; i <= 133; i++) {
		printf("Info: %s Código de error %i\n",strerror(i),i);
	}
	
	

	return 0;

}
