#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main (int argc, char **argv) {
	struct addrinfo hints;
	struct addrinfo *res, *ptr;
	
	memset(&hints, 0, sizeof(struct addrinfo));
	
	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_UNSPEC;

	int rc = getaddrinfo(argv[1], NULL, &hints, &res);

	if(rc != 0) {
		printf("Error getaddrinfo: %s\n", gai_strerror(rc));
		exit (-1);
	}

	for(ptr = res; ptr != NULL; ptr = ptr->ai_next) {
		char host[NI_MAXHOST];
		getnameinfo(ptr->ai_addr, ptr->ai_addrlen, host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
		printf("%s\t%i\t%i\n", host, ptr->ai_family, ptr->ai_socktype);
	}

	freeaddrinfo(res);

	return 0;
}
