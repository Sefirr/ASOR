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
		printf("Use: ./ej1 host port\n");
		return -1;
	}

	struct addrinfo hints;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE;

	struct addrinfo *list;
	int result = getaddrinfo(argv[1], argv[2], &hints, &list);

	if (result != 0) {
		printf("Error while gettinf info: %s\n", gai_strerror(result));
		return -1;
	}

	int on = 1;
	int off = 0;

	int sd = socket(list->ai_family, SOCK_DGRAM, list->ai_protocol);
	if (sd == -1) {
		perror("Unable to open the socket");
		return -1;
	}

	setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (void *)&on, sizeof(int));
	setsockopt(sd, IPPROTO_IPV6, IPV6_V6ONLY, (void *)&off, sizeof(int));

	if (bind(sd, list->ai_addr, list->ai_addrlen)) {
		perror("Unable to bind");
		return -1;
	}

	freeaddrinfo(list);

	char buf[2] = "";
	struct sockaddr_storage input_addr;
	socklen_t input_len = sizeof(input_addr);

	while (buf[0] != 'q') {
		int readsize = recvfrom(sd, &buf, 2*sizeof(char), 0, (struct sockaddr *) &input_addr, &input_len);
		
		if (readsize == -1) {
			perror("Error ocurred while reciving");
			close(sd);
			return -1;
		}
		else if (readsize == 0) {
			printf("Client has performed a shutdown\n");
			buf[0] = 'q';
		}
		else {
			char hostname[NI_MAXHOST];
			char port[NI_MAXSERV];

			if (getnameinfo((struct sockaddr *)&input_addr, input_len, hostname, NI_MAXHOST, port, NI_MAXSERV, 0)) {
				perror("Error getting info from name");
				return -1;
			}

			printf("Recibí %i bytes de %s:%s\n", readsize, hostname, port);

			if (buf[0] == 'd') {
				time_t t;
				struct tm *loctime;
				char buffer[256];
				t =  time(NULL);
				loctime = localtime(&t);
				strftime(buffer,256,"%Y-%m-%d", loctime);
	
				int writesize = sendto(sd, buffer, strlen(buffer), 0, (struct sockaddr *)&input_addr, input_len);
				if (writesize == -1) {
					perror("Error while sending back to client");
					return -1;
				}
			} else if(buf[0] == 't') {
				time_t t;
				struct tm *loctime;
				char buffer[256];
				t =  time(NULL);
				loctime = localtime(&t);
				strftime(buffer,256,"%r", loctime);
	
				int writesize = sendto(sd, buffer, strlen(buffer), 0, (struct sockaddr *)&input_addr, input_len);
				if (writesize == -1) {
					perror("Error while sending back to client");
					return -1;
				}
			} else if (buf[0] == 'q')
				printf("Comando de finalización recibido, saliendo…\n");
			else
				printf("Comando no soportado %s", buf);
		}
	}

	close(sd);
	return 0;
}


