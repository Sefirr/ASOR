#include <stdio.h>
#include <time.h>


int main() {

	time_t t;
	struct tm *loctime;
	char buffer[256];

	t =  time(NULL);
	loctime = localtime(&t);
	strftime(buffer,256,"%Y", loctime);
	printf("Estamos en %s\n",buffer);
	
	return 0;

}
