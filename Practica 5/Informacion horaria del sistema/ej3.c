#include <stdio.h>
#include <time.h>


int main() {

	time_t t;
	struct tm *loctime;
	char buffer[256];
	t =  time(NULL);
	loctime = localtime(&t);
	strftime(buffer,256,"%r", loctime);
	printf("Conversión de la información a cadena usando strftime: %s\n", buffer);
	printf("Conversión de la información a cadena usando ctime: %s\n",ctime(&t));
	return 0;

}
