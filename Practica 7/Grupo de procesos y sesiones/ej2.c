#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdlib.h>

int main(int argc, char **argv) {

	/*if (argc != 2) {
		fprintf(stderr, "Usage: %s <pid>\n", argv[0]);
		exit(EXIT_FAILURE);
	}*/

	pid_t pid = getpid();
	pid_t pgid = getpgid(pid); 
	pid_t ses_id = getsid(pid);
	char dir[1024];
	struct rlimit rlim;
	getrlimit(RLIMIT_NOFILE, &rlim);
	getcwd(dir, sizeof(dir));

	printf("ID del proceso: %i \n", pid);
	printf("PGID del proceso: %i \n", pgid);
	printf("Identificador de sesión %i \n", ses_id);
	printf("Número máximo de ficheros que puede abrir: %lld\n",(long long int)rlim.rlim_max);
	printf("Directorio de trabajo: %s \n", dir);
	
	return 0;

}
