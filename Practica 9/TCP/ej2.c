#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char **argv) {

	if (argc < 3) {
		printf("Use: ./ej2 host port\n");
		return -1;
	}

	struct addrinfo hints;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = 0;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_addr = NULL;
	hints.ai_canonname = NULL;
	hints.ai_next = NULL;

	struct addrinfo *list;
	int result = getaddrinfo(argv[1], argv[2], &hints, &list);

	if (result != 0) {
		printf("Error while gettinf info: %s\n", gai_strerror(result));
		return -1;
	}

	int on = 1;
	int off = 0;

	int sd = socket(list->ai_family, SOCK_STREAM, list->ai_protocol);
	if (sd == -1) {
		perror("Unable to open the socket");
		return -1;
	}

	setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (void *)&on, sizeof(int));
	setsockopt(sd, IPPROTO_IPV6, IPV6_V6ONLY, (void *)&off, sizeof(int));

	if (connect(sd, list->ai_addr, list->ai_addrlen)) {
		perror("Unable to connect");
		close(sd);
		return -1;
	}

	freeaddrinfo(list);

	char buf[257] = "";
	ssize_t readsize, writesize;

	while (strcmp("Q\n", buf)) {
		readsize = read(0, buf, 256*sizeof(char));
		if (readsize == -1) {
			perror("Error ocurred while reading");
			close(sd);
			return -1;
		}

		buf[readsize] = '\0';
		writesize = send(sd, buf, (readsize + 1)*sizeof(char), 0);
		if (writesize == -1) {
			perror("Error ocurred while sending");
			close(sd);
			return -1;
		}

		readsize = recv(sd, buf, 256*sizeof(char), 0);
		if (readsize == -1) {
			perror("Error ocurred while reading");
			close(sd);
			return -1;
		}
		buf[readsize] = '\0';
		printf("%s", buf);
	}

	close(sd);
	return 0;
}

