#include <sched.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {

	int sch_num = sched_getscheduler(0);
	if (sch_num == -1) {
		perror("Unable to get the scheduler");
		return -1;
	}

	int prior_min = sched_get_priority_min(SCHED_RR);
	int prior_max = sched_get_priority_max(SCHED_RR);
	int prior = prior_max;

	if (argc == 2) {
		int prior_attr = atoi(argv[1]);
		if (prior_min <= prior_attr && prior_attr <= prior_max)
			prior = prior_attr;
	}
	
	struct sched_param prior_param;
	int rc_prior = sched_getparam(0, &prior_param);
	if (rc_prior == -1) {
		perror("Unable to get the scheduler priority");
		return -1;
	}
	prior_param.sched_priority = prior;
	int rc = sched_setscheduler(0, SCHED_RR, &prior_param);
	if (rc == -1) {
		perror("Unable to set the scheduler");
		return -1;
	}

	printf("Starting super-shell:\n");
	system("bash -c 'while true; do echo HOLA; done'");

	return 0;
}

