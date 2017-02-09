#include <sched.h>
#include <sys/types.h>
#include <stdio.h>

int main(int argc, char **argv) {
	
	int policy;
	struct sched_param params;
	
	policy = sched_getscheduler(0);
	sched_getparam(0, &params);
	
	int max_priority = sched_get_priority_max(policy);
	int min_priority = sched_get_priority_min(policy);
	printf("Proceso actual: %i\n",getpid());
	if(policy == 0) {
		printf("Política del proceso: SCHED_OTHER\nPrioridad: %i\n",params.sched_priority);
	} else if(policy == 1) {
		printf("Política del proceso: SCHED_FIFO\nPrioridad: %i\n",params.sched_priority);
	} else if(policy == 2) {
		printf("Política del proceso: SCHED_RR\nPrioridad: %i\n",params.sched_priority);
	}

	printf("Prioridad máxima: %i Prioridad minima: %i\n", max_priority, min_priority);


	return 0;

}
