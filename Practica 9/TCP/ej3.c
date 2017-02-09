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

	if (argc < 2) {
		printf("Use: ./ej1 port\n");
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
	int result = getaddrinfo("::", argv[1], &hints, &list);

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

	if (bind(sd, list->ai_addr, list->ai_addrlen)) {
		perror("Unable to bind");
		close(sd);
		return -1;
	}

	freeaddrinfo(list);

	char buf[257] = "";
	struct sockaddr_storage input_addr;
	socklen_t input_len = sizeof(input_addr);

	if (-1 == listen(sd, 10)) {
		perror("Error while listening");
		return -1;
	}

	pid_t pid = getpid();
	while (pid) {

		int asock = accept(sd, (struct sockaddr*)&input_addr, &input_len);
		if (asock == -1) {
			perror("Error while accepting connection");
			exit(-1);
		}

		char hostname[NI_MAXHOST];
		char port[NI_MAXSERV];
		if (getnameinfo((struct sockaddr*)&input_addr, input_len, hostname, NI_MAXHOST, port, NI_MAXSERV, 0)) {
			perror("Error getting info from name");
			exit(-1);
		}
		printf("Connection from %s : %s\n", hostname, port);

		pid = fork();
		if (pid == -1) {
			perror("Unable to fork");
			exit(-1);
		}

		if (pid == 0) {
			ssize_t readsize;

			do {
				readsize = recv(asock, buf, 256*sizeof(char), 0);
					
				if (readsize == -1) {
					perror("Error ocurred while reciving");
					close(sd);
					exit(-1);
				}
				else if (readsize == 0) {
					printf("Client has performed a shutdown\n");
				}
				else {
					buf[readsize] = '\0';
					ssize_t writesize = send(asock, buf, (readsize + 1)*sizeof(char), 0);
					if (writesize == -1) {
						perror("Error while sending back to client");
						exit(-1);
					}
				}

			} while (readsize > 0);
		}

		close(asock);
	}

	close(sd);
	exit(0);
	return 0;
}

