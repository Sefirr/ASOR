#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>



int main() {

	printf("ID real: %i ID efectivo: %i\n",geteuid(),getuid());


	return 0;
}
