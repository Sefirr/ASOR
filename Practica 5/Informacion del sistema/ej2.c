#include <sys/utsname.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
	struct utsname user;

	if(uname(&user) == -1) {
		perror("uname(&user)");
		exit -1;
	}

	printf("Name's machine: %s \nOperating system: %s \nKernel Release: %s Kernel Version: %s \nHardware identiffier: %s \n",user.nodename, user.sysname, user.release, user.version, user.machine);
}
